#include "AbilitySystem/AbilityAsync/FuAbilityAsync_EffectStackListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_EffectStackListener)

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::FuListenForEffectStackChangeOnActor(
	const AActor* Actor, const TSubclassOf<UGameplayEffect> EffectClass)
{
	return FuListenForEffectStackChange(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)),
		EffectClass);
}

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::FuListenForEffectStackChange(
	UFuAbilitySystemComponent* AbilitySystem, const TSubclassOf<UGameplayEffect> EffectClass)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->EffectClass1 = EffectClass;

	return Task;
}

void UFuAbilityAsync_EffectStackListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass1)) ||
	    !FU_ENSURE(EffectClass1.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	const auto bDelegatesBroadcastAllowed{ShouldBroadcastDelegates()};
	auto bAnyEffectValid{false};

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
	FScopedActiveGameplayEffectLock EffectScopeLock{AbilitySystem->GetActiveEffects()};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
	{
		if (ActiveEffect.Spec.Def->GetClass() != EffectClass1)
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
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		for (auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
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
	if (ShouldBroadcastDelegates() && EffectSpecification.Def->GetClass() == EffectClass1)
	{
		AbilitySystem->OnGameplayEffectStackChangeDelegate(EffectHandle)->AddUObject(this, &ThisClass::AbilitySystem_OnEffectStackChanged);

		OnStackChanged.Broadcast(EffectHandle, EffectSpecification.StackCount, 0);
	}
}

void UFuAbilityAsync_EffectStackListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	if (ActiveEffect.Spec.Def->GetClass() == EffectClass1)
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
