#include "AbilitySystem/AbilityTasks//FuAbilityTask_EffectStackListener.h"

#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

UFuAbilityTask_EffectStackListener* UFuAbilityTask_EffectStackListener::FuWaitForEffectStackChange(
	UGameplayAbility* OwningAbility, const TSubclassOf<UGameplayEffect> EffectClass)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->EffectClass1 = EffectClass;

	return Task;
}

void UFuAbilityTask_EffectStackListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(AbilitySystemComponent)};

	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass1)) ||
	    !FU_ENSURE(EffectClass1.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		EndTask();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnActiveGameplayEffectRemoved);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastAbilityTaskDelegates()};
	auto bAnyEffectValid{false};

	FScopedActiveGameplayEffectLock EffectScopeLock{AbilitySystem->GetActiveEffects()};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
	{
		if (ActiveEffect.Spec.Def->GetClass() != EffectClass1)
		{
			continue;
		}

		bAnyEffectValid = true;

		AbilitySystem->OnGameplayEffectStackChangeDelegate(ActiveEffect.Handle)->AddUObject(this, &ThisClass::OnEffectStackChanged);

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
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(AbilitySystemComponent)};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		for (auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
		{
			ActiveEffect.EventSet.OnStackChanged.RemoveAll(this);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_EffectStackListener::OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                     const FGameplayEffectSpec& EffectSpecification,
                                                                     const FActiveGameplayEffectHandle EffectHandle) const
{
	if (EffectSpecification.Def->GetClass() == EffectClass1)
	{
		AbilitySystem->OnGameplayEffectStackChangeDelegate(EffectHandle)->AddUObject(this, &ThisClass::OnEffectStackChanged);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnStackChanged.Broadcast(EffectHandle, EffectSpecification.StackCount, 0);
		}
	}
}

void UFuAbilityTask_EffectStackListener::OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	if (ActiveEffect.Spec.Def->GetClass() == EffectClass1)
	{
		const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnStackChanged.RemoveAll(this);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnStackChanged.Broadcast(ActiveEffect.Handle, 0, ActiveEffect.Spec.StackCount);
		}
	}
}

void UFuAbilityTask_EffectStackListener::OnEffectStackChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                              const int32 NewCount, const int32 PreviousCount) const
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnStackChanged.Broadcast(EffectHandle, NewCount, PreviousCount);
	}
}
