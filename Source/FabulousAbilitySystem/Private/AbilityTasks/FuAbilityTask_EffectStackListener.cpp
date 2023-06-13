#include "AbilityTasks//FuAbilityTask_EffectStackListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_EffectStackListener)

UFuAbilityTask_EffectStackListener* UFuAbilityTask_EffectStackListener::FuWaitForEffectStackChange(
	UGameplayAbility* OwningAbility, const TSubclassOf<UGameplayEffect> InEffectClass)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->EffectClass = InEffectClass;

	return Task;
}

void UFuAbilityTask_EffectStackListener::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(IsValid(EffectClass)) || !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		EndTask();
		return;
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
	                      .AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);

	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastAbilityTaskDelegates()};
	auto bAnyEffectValid{false};

	FScopedActiveGameplayEffectLock EffectScopeLock{
		const_cast<FActiveGameplayEffectsContainer&>(AbilitySystemComponent->GetActiveGameplayEffects())
	};

	for (const auto& ActiveEffect : &AbilitySystemComponent->GetActiveGameplayEffects())
	{
		// The backing gameplay effect class must be exactly the same. This is required for
		// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

		if (ActiveEffect.Spec.Def->GetClass() != EffectClass)
		{
			continue;
		}

		bAnyEffectValid = true;

		AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(ActiveEffect.Handle)
		                      ->AddUObject(this, &ThisClass::AbilitySystem_OnEffectStackChanged);

		if (bDelegatesBroadcastAllowed)
		{
			OnStackChanged.Broadcast(ActiveEffect.Handle, ActiveEffect.Spec.StackCount, 0);
		}
	}

	if (bDelegatesBroadcastAllowed && !bAnyEffectValid)
	{
		OnStackChanged.Broadcast({}, 0, 0);
	}
}

void UFuAbilityTask_EffectStackListener::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		for (auto& ActiveEffect : const_cast<FActiveGameplayEffectsContainer*>(&AbilitySystemComponent->GetActiveGameplayEffects()))
		{
			ActiveEffect.EventSet.OnStackChanged.RemoveAll(this);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_EffectStackListener::AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                                   const FGameplayEffectSpec& EffectSpecification,
                                                                                   const FActiveGameplayEffectHandle EffectHandle) const
{
	// The backing gameplay effect class must be exactly the same. This is required for
	// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

	if (EffectSpecification.Def->GetClass() == EffectClass)
	{
		AbilitySystem->OnGameplayEffectStackChangeDelegate(EffectHandle)->AddUObject(this, &ThisClass::AbilitySystem_OnEffectStackChanged);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnStackChanged.Broadcast(EffectHandle, EffectSpecification.StackCount, 0);
		}
	}
}

void UFuAbilityTask_EffectStackListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	// The backing gameplay effect class must be exactly the same. This is required for
	// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

	if (ActiveEffect.Spec.Def->GetClass() == EffectClass)
	{
		const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnStackChanged.RemoveAll(this);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnStackChanged.Broadcast(ActiveEffect.Handle, 0, ActiveEffect.Spec.StackCount);
		}
	}
}

void UFuAbilityTask_EffectStackListener::AbilitySystem_OnEffectStackChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                                            const int32 NewCount, const int32 PreviousCount) const
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnStackChanged.Broadcast(EffectHandle, NewCount, PreviousCount);
	}
}
