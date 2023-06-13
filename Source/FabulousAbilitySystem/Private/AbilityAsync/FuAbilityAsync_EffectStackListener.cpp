#include "AbilityAsync/FuAbilityAsync_EffectStackListener.h"

#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_EffectStackListener)

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::FuListenForEffectStackChangeOnActor(
	const AActor* Actor, const TSubclassOf<UGameplayEffect> InEffectClass)
{
	return FuListenForEffectStackChange(UFuAbilitySystemUtility::GetAbilitySystem(Actor), InEffectClass);
}

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::FuListenForEffectStackChange(
	UAbilitySystemComponent* AbilitySystem, const TSubclassOf<UGameplayEffect> InEffectClass)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->EffectClass = InEffectClass;

	return Task;
}

void UFuAbilityAsync_EffectStackListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || !FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass)) ||
	    !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastDelegates()};
	auto bAnyEffectValid{false};

	FScopedActiveGameplayEffectLock EffectScopeLock{
		const_cast<FActiveGameplayEffectsContainer&>(AbilitySystem->GetActiveGameplayEffects())
	};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		// The backing gameplay effect class must be exactly the same. This is required for
		// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

		if (ActiveEffect.Spec.Def->GetClass() != EffectClass)
		{
			continue;
		}

		bAnyEffectValid = true;

		AbilitySystem->OnGameplayEffectStackChangeDelegate(ActiveEffect.Handle)
		             ->AddUObject(this, &ThisClass::AbilitySystem_OnEffectStackChanged);

		if (bDelegatesBroadcastAllowed)
		{
			OnStackChanged.Broadcast(ActiveEffect.Handle, ActiveEffect.Spec.StackCount, 0);
		}
	}

	if (!bDelegatesBroadcastAllowed || !bAnyEffectValid)
	{
		OnStackChanged.Broadcast({}, 0, 0);
	}
}

void UFuAbilityAsync_EffectStackListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystem->GetActiveGameplayEffects()))
		{
			ActiveEffect.EventSet.OnStackChanged.RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_EffectStackListener::AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                                    const FGameplayEffectSpec& EffectSpecification,
                                                                                    const FActiveGameplayEffectHandle EffectHandle) const
{
	// The backing gameplay effect class must be exactly the same. This is required for
	// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

	if (ShouldBroadcastDelegates() && EffectSpecification.Def->GetClass() == EffectClass)
	{
		AbilitySystem->OnGameplayEffectStackChangeDelegate(EffectHandle)->AddUObject(this, &ThisClass::AbilitySystem_OnEffectStackChanged);

		OnStackChanged.Broadcast(EffectHandle, EffectSpecification.StackCount, 0);
	}
}

void UFuAbilityAsync_EffectStackListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	// The backing gameplay effect class must be exactly the same. This is required for
	// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

	if (ActiveEffect.Spec.Def->GetClass() == EffectClass)
	{
		const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnStackChanged.RemoveAll(this);

		if (ShouldBroadcastDelegates())
		{
			OnStackChanged.Broadcast(ActiveEffect.Handle, 0, ActiveEffect.Spec.StackCount);
		}
	}
}

void UFuAbilityAsync_EffectStackListener::AbilitySystem_OnEffectStackChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                                             const int32 NewCount, const int32 PreviousCount) const
{
	if (ShouldBroadcastDelegates())
	{
		OnStackChanged.Broadcast(EffectHandle, NewCount, PreviousCount);
	}
}
