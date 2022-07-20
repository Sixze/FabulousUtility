#include "AbilitySystem/AbilityAsync/FuAbilityAsync_EffectStackListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "GameplayEffect.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

UFuAbilityAsync_EffectStackListener* UFuAbilityAsync_EffectStackListener::FuListenForEffectStackChangeActor(
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
		SetReadyToDestroy();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnActiveGameplayEffectRemoved);

	auto bAnyEffectValid{false};

	for (const auto& ActiveEffect : AbilitySystem->GetActiveEffects())
	{
		bAnyEffectValid = bAnyEffectValid || ActiveEffect.Spec.Def->GetClass() == EffectClass1;

		OnActiveGameplayEffectAdded(AbilitySystem, ActiveEffect.Spec, ActiveEffect.Handle);
	}

	if (ShouldBroadcastDelegates() && !bAnyEffectValid)
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

		for (auto& ActiveEffect : AbilitySystem->GetActiveEffectsMutable())
		{
			ActiveEffect.EventSet.OnStackChanged.RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_EffectStackListener::OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                      const FGameplayEffectSpec& EffectSpecification,
                                                                      const FActiveGameplayEffectHandle EffectHandle) const
{
	if (ShouldBroadcastDelegates() && EffectSpecification.Def->GetClass() == EffectClass1)
	{
		AbilitySystem->OnGameplayEffectStackChangeDelegate(EffectHandle)
		             ->AddUObject(this, &ThisClass::OnEffectStackChanged);

		OnStackChanged.Broadcast(EffectHandle, EffectSpecification.StackCount, 0);
	}
}

void UFuAbilityAsync_EffectStackListener::OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	if (ShouldBroadcastDelegates() && ActiveEffect.Spec.Def->GetClass() == EffectClass1)
	{
		const_cast<FActiveGameplayEffect&>(ActiveEffect).EventSet.OnStackChanged.RemoveAll(this);

		OnStackChanged.Broadcast(ActiveEffect.Handle, 0, ActiveEffect.Spec.StackCount);
	}
}

void UFuAbilityAsync_EffectStackListener::OnEffectStackChanged(const FActiveGameplayEffectHandle EffectHandle,
                                                               const int32 NewCount, const int32 PreviousCount) const
{
	if (ShouldBroadcastDelegates())
	{
		OnStackChanged.Broadcast(EffectHandle, NewCount, PreviousCount);
	}
}
