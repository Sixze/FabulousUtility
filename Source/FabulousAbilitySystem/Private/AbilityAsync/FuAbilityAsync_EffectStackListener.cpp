#include "AbilityAsync/FuAbilityAsync_EffectStackListener.h"

#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_EffectStackListener)

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::ListenForEffectStackChangeOnActor(
	const AActor* Actor, const TSubclassOf<UGameplayEffect> InEffectClass)
{
	return ListenForEffectStackChange(UFuAbilitySystemUtility::GetAbilitySystem(Actor), InEffectClass);
}

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::ListenForEffectStackChangeOnActorSoft(
	const AActor* Actor, const TSoftClassPtr<UGameplayEffect> InEffectClass)
{
	return ListenForEffectStackChangeSoft(UFuAbilitySystemUtility::GetAbilitySystem(Actor), InEffectClass);
}

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::ListenForEffectStackChange(
	UAbilitySystemComponent* AbilitySystem, const TSubclassOf<UGameplayEffect> InEffectClass)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(IsValid(InEffectClass)))
	{
		Task->EffectClass = InEffectClass;
	}

	return Task;
}

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::ListenForEffectStackChangeSoft(
	UAbilitySystemComponent* AbilitySystem, const TSoftClassPtr<UGameplayEffect> InEffectClass)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(!InEffectClass.IsNull()))
	{
		// If the effect is not loaded, then there are no active effects.
		Task->EffectClass = InEffectClass.Get();
	}

	return Task;
}

void UFuAbilityAsync_EffectStackListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || !IsValid(EffectClass) ||
	    !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastDelegates()};
	auto bAnyEffectValid{false};

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
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
			OnStackChanged.Broadcast(ActiveEffect.Handle, ActiveEffect.Spec.GetStackCount(), 0);
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

		OnStackChanged.Broadcast(EffectHandle, EffectSpecification.GetStackCount(), 0);
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
			OnStackChanged.Broadcast(ActiveEffect.Handle, 0, ActiveEffect.Spec.GetStackCount());
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
