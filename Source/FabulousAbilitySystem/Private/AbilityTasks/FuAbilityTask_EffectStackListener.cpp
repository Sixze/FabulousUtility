#include "AbilityTasks//FuAbilityTask_EffectStackListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_EffectStackListener)

UFuAbilityTask_EffectStackListener* UFuAbilityTask_EffectStackListener::WaitForEffectStackChange(
	UGameplayAbility* OwningAbility, const TSubclassOf<UGameplayEffect> InEffectClass)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(IsValid(InEffectClass)))
	{
		Task->EffectClass = InEffectClass;
	}

	return Task;
}

UFuAbilityTask_EffectStackListener* UFuAbilityTask_EffectStackListener::WaitForEffectStackChangeSoft(
	UGameplayAbility* OwningAbility, const TSoftClassPtr<UGameplayEffect> InEffectClass)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(!InEffectClass.IsNull()))
	{
		// If the effect is not loaded, then there are no active effects.
		Task->EffectClass = InEffectClass.Get();
	}

	return Task;
}

void UFuAbilityTask_EffectStackListener::Activate()
{
	Super::Activate();

	// TODO Replace StackingType with UGameplayEffect::GetStackingType() in future engine versions.
	PRAGMA_DISABLE_DEPRECATION_WARNINGS

	if (!IsValid(EffectClass) || !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		EndTask();
		return;
	}

	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
	                      .AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);

	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastAbilityTaskDelegates()};
	auto bAnyEffectValid{false};

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
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
			OnStackChanged.Broadcast(ActiveEffect.Handle, ActiveEffect.Spec.GetStackCount(), 0);
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
			OnStackChanged.Broadcast(EffectHandle, EffectSpecification.GetStackCount(), 0);
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
			OnStackChanged.Broadcast(ActiveEffect.Handle, 0, ActiveEffect.Spec.GetStackCount());
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
