#include "AbilityAsync/FuAbilityAsync_EffectTimeListener.h"

#include "FuAbilitySystemComponent.h"
#include "FuMacros.h"
#include "Utility/FuEffectSpecificationUtility.h"
#include "Utility/FuEffectUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_EffectTimeListener)

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::ListenForEffectTimeChangeOnActor(
	const AActor* Actor, const FGameplayTag InEffectTag, const bool bAllowPredictedTime)
{
	return ListenForEffectTimeChange(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor), InEffectTag, bAllowPredictedTime);
}

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::ListenForEffectsTimeChangeOnActor(
	const AActor* Actor, const FGameplayTagContainer InEffectTags, const bool bAllowPredictedTime)
{
	return ListenForEffectsTimeChange(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor), InEffectTags, bAllowPredictedTime);
}

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::ListenForEffectTimeChange(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag InEffectTag, const bool bAllowPredictedTime)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(InEffectTag.IsValid()))
	{
		Task->EffectTags.AddTag(InEffectTag);
	}

	Task->bPredictedTimeAllowed = bAllowPredictedTime;

	return Task;
}

UFuAbilityAsync_EffectTimeListener* UFuAbilityAsync_EffectTimeListener::ListenForEffectsTimeChange(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer InEffectTags, const bool bAllowPredictedTime)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : InEffectTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->EffectTags.AddTag(Tag);
		}
	}

	Task->bPredictedTimeAllowed = bAllowPredictedTime;

	return Task;
}

void UFuAbilityAsync_EffectTimeListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || EffectTags.IsEmpty())
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	for (const auto& EffectTag : EffectTags)
	{
		AbilitySystem->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved)
		             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);
	}

	for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
	{
		if (UFuEffectSpecificationUtility::HasAnyGrantedTags(ActiveEffect.Spec, EffectTags))
		{
			ActiveEffect.EventSet.OnTimeChanged.AddUObject(this, &ThisClass::ActiveEffect_OnTimeChanged);
		}
	}

	for (const auto& EffectTag : EffectTags)
	{
		BroadcastEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_EffectTimeListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		for (const auto& EffectTag : EffectTags)
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

void UFuAbilityAsync_EffectTimeListener::BroadcastEffectTimeRemainingAndDurationForTag(const FGameplayTag& EffectTag) const
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

	// A non-replicated ability instance is only valid for predicted effects on the client.

	const auto bPredictedTime{
		AbilitySystem->GetOwnerRole() <= ROLE_AutonomousProxy && IsValid(ActiveEffect->Spec.GetContext().GetAbilityInstance_NotReplicated())
	};

	if (bPredictedTimeAllowed || !bPredictedTime)
	{
		OnEffectStarted.Broadcast(EffectTag, TimeRemaining, Duration, bPredictedTime);
	}
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UFuAbilityAsync_EffectTimeListener::AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                                   const FGameplayEffectSpec& EffectSpecification,
                                                                                   const FActiveGameplayEffectHandle EffectHandle) const
{
	auto bEffectTimeChangeEventRegistered{false};

	for (const auto& EffectTag : EffectTags)
	{
		if (!UFuEffectSpecificationUtility::HasGrantedTag(EffectSpecification, EffectTag))
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

		BroadcastEffectTimeRemainingAndDurationForTag(EffectTag);
	}
}

void UFuAbilityAsync_EffectTimeListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnTimeChanged.RemoveAll(this);
}

void UFuAbilityAsync_EffectTimeListener::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 TagCount) const
{
	if (ShouldBroadcastDelegates() && TagCount <= 0)
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

	FScopedActiveGameplayEffectLock EffectScopeLock{
		const_cast<FActiveGameplayEffectsContainer&>(AbilitySystem->GetActiveGameplayEffects())
	};

	for (const auto& EffectTag : EffectTags)
	{
		if (UFuEffectSpecificationUtility::HasGrantedTag(ActiveEffect->Spec, EffectTag))
		{
			BroadcastEffectTimeRemainingAndDurationForTag(EffectTag);
		}
	}
}
