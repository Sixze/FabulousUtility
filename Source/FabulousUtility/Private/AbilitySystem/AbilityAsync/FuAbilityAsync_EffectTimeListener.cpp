#include "AbilitySystem/AbilityAsync/FuAbilityAsync_EffectTimeListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"
#include "AbilitySystem/Utility/FuEffectUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_EffectTimeListener)

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::FuListenForEffectTimeChangeOnActor(
	const AActor* Actor, const FGameplayTag EffectTag, const bool bWaitForTimeFromServer)
{
	return FuListenForEffectTimeChange(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)),
		EffectTag, bWaitForTimeFromServer);
}

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::FuListenForEffectsTimeChangeOnActor(
	const AActor* Actor, const FGameplayTagContainer EffectTags, const bool bWaitForTimeFromServer)
{
	return FuListenForEffectsTimeChange(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)),
		EffectTags, bWaitForTimeFromServer);
}

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::FuListenForEffectTimeChange(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag EffectTag, const bool bWaitForTimeFromServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(EffectTag.IsValid()))
	{
		Task->EffectTags1.AddTag(EffectTag);
	}

	Task->bWaitForTimeFromServer1 = bWaitForTimeFromServer;

	return Task;
}

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::FuListenForEffectsTimeChange(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer EffectTags, const bool bWaitForTimeFromServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : EffectTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->EffectTags1.AddTag(Tag);
		}
	}

	Task->bWaitForTimeFromServer1 = bWaitForTimeFromServer;

	return Task;
}

void UFuAbilityAsync_EffectTimeListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || !FU_ENSURE(IsValid(AbilitySystem)) || EffectTags1.IsEmpty())
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	for (const auto& EffectTag : EffectTags1)
	{
		AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved)
		             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);
	}

	for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
	{
		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags1) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags1))
		{
			ActiveEffect.EventSet.OnTimeChanged.AddUObject(this, &ThisClass::ActiveEffect_OnTimeChanged);
		}
	}

	for (const auto& EffectTag : EffectTags1)
	{
		RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_EffectTimeListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		for (const auto& EffectTag : EffectTags1)
		{
			AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}

		for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
		{
			ActiveEffect.EventSet.OnTimeChanged.RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_EffectTimeListener::RefreshEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const
{
	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	const auto* AbilitySystem{GetAbilitySystemComponent()};

	float TimeRemaining, Duration;

	const auto* ActiveEffect{
		UFuEffectUtility::GetActiveEffectTimeRemainingAndDurationByTag(AbilitySystem, EffectTag, TimeRemaining, Duration)
	};

	if (ActiveEffect == nullptr)
	{
		if (!AbilitySystem->HasMatchingGameplayTag(EffectTag))
		{
			OnEffectEnded.Broadcast(EffectTag, 0.0f, 0.0f, false);
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

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UFuAbilityAsync_EffectTimeListener::AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                                   const FGameplayEffectSpec& EffectSpecification,
                                                                                   const FActiveGameplayEffectHandle EffectHandle) const
{
	auto bEffectTimeChangeEventRegistered{false};

	for (const auto& EffectTag : EffectTags1)
	{
		if (!EffectSpecification.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) &&
		    !EffectSpecification.DynamicGrantedTags.HasTag(EffectTag))
		{
			continue;
		}

		if (!bEffectTimeChangeEventRegistered)
		{
			auto* ActiveEffect{AbilitySystem->GetActiveGameplayEffect(EffectHandle)};
			if (ActiveEffect != nullptr)
			{
				const_cast<FActiveGameplayEffect*>(ActiveEffect)
					->EventSet.OnTimeChanged.AddUObject(this, &ThisClass::ActiveEffect_OnTimeChanged);

				bEffectTimeChangeEventRegistered = true;
			}
		}

		RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_EffectTimeListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnTimeChanged.RemoveAll(this);
}

void UFuAbilityAsync_EffectTimeListener::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 Count) const
{
	if (ShouldBroadcastDelegates() && Count <= 0)
	{
		OnEffectEnded.Broadcast(Tag, 0.0f, 0.0f, false);
	}
}

void UFuAbilityAsync_EffectTimeListener::ActiveEffect_OnTimeChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                                    const float StartTime, const float Duration) const
{
	const auto* AbilitySystem{GetAbilitySystemComponent()};

	auto* ActiveEffect{AbilitySystem->GetActiveGameplayEffect(EffectHandle)};
	if (ActiveEffect == nullptr)
	{
		return;
	}

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
	FScopedActiveGameplayEffectLock EffectScopeLock{
		const_cast<FActiveGameplayEffectsContainer&>(AbilitySystem->GetActiveGameplayEffects())};

	for (const auto& EffectTag : EffectTags1)
	{
		if (ActiveEffect->Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect->Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			RefreshEffectTimeRemainingAndDurationForTag(EffectTag);
		}
	}
}
