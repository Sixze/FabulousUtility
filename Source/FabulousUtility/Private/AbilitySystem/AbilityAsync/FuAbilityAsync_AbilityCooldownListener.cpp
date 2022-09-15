#include "AbilitySystem/AbilityAsync/FuAbilityAsync_AbilityCooldownListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"
#include "AbilitySystem/Utility/FuEffectUtility.h"

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldownActor(
	const AActor* Actor, const int32 InputId, const bool bWaitForTimeFromServer)
{
	return FuListenForAbilityCooldown(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)),
		InputId, bWaitForTimeFromServer);
}

UFuAbilityAsync_AbilityCooldownListener* UFuAbilityAsync_AbilityCooldownListener::FuListenForAbilityCooldown(
	UFuAbilitySystemComponent* AbilitySystem, const int32 InputId, const bool bWaitForTimeFromServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->InputId1 = InputId;
	Task->bWaitForTimeFromServer1 = bWaitForTimeFromServer;

	return Task;
}

void UFuAbilityAsync_AbilityCooldownListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(InputId1 > 0))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnActiveGameplayEffectRemoved);

	AbilitySystem->OnAbilityGiven.AddUObject(this, &ThisClass::OnAbilityGiven);
	AbilitySystem->OnAbilityRemoved.AddUObject(this, &ThisClass::OnAbilityRemoved);

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.InputID == InputId1)
		{
			const auto* CooldownTags{AbilitySpecification.Ability->GetCooldownTags()};
			if (CooldownTags != nullptr)
			{
				EffectTags.UpdateTagCount(*CooldownTags, 1);
			}
		}
	}

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved)
		             .AddUObject(this, &ThisClass::OnEffectTagChanged);
	}

	for (auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
	{
		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags.GetExplicitGameplayTags()) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags.GetExplicitGameplayTags()))
		{
			ActiveEffect.EventSet.OnTimeChanged.AddUObject(this, &ThisClass::OnEffectTimeChanged);
		}
	}

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_AbilityCooldownListener::EndAction()
{
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		AbilitySystem->OnAbilityGiven.RemoveAll(this);
		AbilitySystem->OnAbilityRemoved.RemoveAll(this);

		for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
		{
			AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}

		for (auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
		{
			ActiveEffect.EventSet.OnTimeChanged.RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_AbilityCooldownListener::ProcessAbilitySpecificationChange(const FGameplayAbilitySpec& AbilitySpecification,
                                                                                const bool bAddedOrRemoved)
{
	if (AbilitySpecification.InputID != InputId1)
	{
		return;
	}

	const auto* CooldownTags{AbilitySpecification.Ability->GetCooldownTags()};

	if (CooldownTags == nullptr || CooldownTags->IsEmpty())
	{
		return;
	}

	EffectTags.UpdateTagCount(*CooldownTags, bAddedOrRemoved ? 1 : -1);

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	for (const auto& CooldownTag : *CooldownTags)
	{
		const auto TagCount{EffectTags.GetTagCount(CooldownTag)};

		if (bAddedOrRemoved && TagCount == 1)
		{
			// A cooldown tag has been added.

			AbilitySystem->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
			             .AddUObject(this, &ThisClass::OnEffectTagChanged);

			RefreshEffectTimeRemainingAndDurationForTag(CooldownTag);
		}
		else if (!bAddedOrRemoved && TagCount <= 0)
		{
			// A cooldown tag has been removed.

			AbilitySystem->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
			             .AddUObject(this, &ThisClass::OnEffectTagChanged);

			if (ShouldBroadcastDelegates())
			{
				OnEffectEnded.Broadcast(CooldownTag, -1.0f, -1.0f, false);
			}
		}
	}

	// Re-register effect time change events.

	for (auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
	{
		ActiveEffect.EventSet.OnTimeChanged.RemoveAll(this);

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags.GetExplicitGameplayTags()) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags.GetExplicitGameplayTags()))
		{
			ActiveEffect.EventSet.OnTimeChanged.AddUObject(this, &ThisClass::OnEffectTimeChanged);
		}
	}
}

void UFuAbilityAsync_AbilityCooldownListener::RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const
{
	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	const auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	float TimeRemaining, Duration;

	const auto* ActiveEffect{
		UFuEffectUtility::GetActiveEffectTimeRemainingAndDurationByTag(AbilitySystem, EffectTag, TimeRemaining, Duration)
	};

	if (ActiveEffect == nullptr)
	{
		if (!AbilitySystem->HasMatchingGameplayTag(EffectTag))
		{
			OnEffectEnded.Broadcast(EffectTag, -1.0f, -1.0f, false);
		}

		return;
	}

	if (AbilitySystem->GetOwnerRole() >= ROLE_Authority)
	{
		OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, false);
	}
	else if (IsValid(ActiveEffect->Spec.GetContext().GetAbilityInstance_NotReplicated()))
	{
		if (bWaitForTimeFromServer1)
		{
			// Waiting for time from the server.
			OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, true);
		}
		else
		{
			// Predicted time.
			OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, false);
		}
	}
	else if (bWaitForTimeFromServer1)
	{
		// Time from the server.
		OnEffectStated.Broadcast(EffectTag, TimeRemaining, Duration, false);
	}
}

void UFuAbilityAsync_AbilityCooldownListener::OnAbilityGiven(UFuAbilitySystemComponent* AbilitySystem,
                                                             const FGameplayAbilitySpec& AbilitySpecification)
{
	ProcessAbilitySpecificationChange(AbilitySpecification, true);
}

void UFuAbilityAsync_AbilityCooldownListener::OnAbilityRemoved(UFuAbilitySystemComponent* AbilitySystem,
                                                               const FGameplayAbilitySpec& AbilitySpecification)
{
	ProcessAbilitySpecificationChange(AbilitySpecification, false);
}

void UFuAbilityAsync_AbilityCooldownListener::OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                          const FGameplayEffectSpec& EffectSpecification,
                                                                          const FActiveGameplayEffectHandle EffectHandle) const
{
	auto bEffectTimeChangeEventRegistered{false};

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		if (EffectSpecification.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    EffectSpecification.DynamicGrantedTags.HasTag(EffectTag))
		{
			if (!bEffectTimeChangeEventRegistered)
			{
				auto* ActiveEffect{AbilitySystem->GetActiveGameplayEffect(EffectHandle)};
				if (ActiveEffect != nullptr)
				{
					const_cast<FActiveGameplayEffect*>(ActiveEffect)->EventSet.OnTimeChanged
					                                                .AddUObject(this, &ThisClass::OnEffectTimeChanged);

					bEffectTimeChangeEventRegistered = true;
				}
			}

			RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
		}
	}
}

void UFuAbilityAsync_AbilityCooldownListener::OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnTimeChanged.RemoveAll(this);
}

void UFuAbilityAsync_AbilityCooldownListener::OnEffectTimeChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                                  const float NewStartTime, const float NewDuration) const
{
	auto* ActiveEffect{GetAbilitySystemComponent()->GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect == nullptr)
	{
		return;
	}

	for (const auto& EffectTag : EffectTags.GetExplicitGameplayTags())
	{
		if (ActiveEffect->Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect->Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
		}
	}
}

void UFuAbilityAsync_AbilityCooldownListener::OnEffectTagChanged(const FGameplayTag EffectTag, const int32 NewCount) const
{
	if (ShouldBroadcastDelegates() && NewCount <= 0)
	{
		OnEffectEnded.Broadcast(EffectTag, -1.0f, -1.0f, false);
	}
}
