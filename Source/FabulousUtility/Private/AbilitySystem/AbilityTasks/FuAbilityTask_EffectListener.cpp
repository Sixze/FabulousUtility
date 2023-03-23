#include "AbilitySystem/AbilityTasks/FuAbilityTask_EffectListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_EffectListener)

UFuAbilityTask_EffectListener* UFuAbilityTask_EffectListener::FuWaitForEffectChangeByTag(UGameplayAbility* OwningAbility,
                                                                                         const FGameplayTag EffectTag)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(EffectTag.IsValid()))
	{
		Task->EffectTags1.AddTag(EffectTag);
	}

	return Task;
}

UFuAbilityTask_EffectListener* UFuAbilityTask_EffectListener::FuWaitForEffectChangeByTags(UGameplayAbility* OwningAbility,
                                                                                          const FGameplayTagContainer EffectTags)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	for (const auto& Tag : EffectTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->EffectTags1.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityTask_EffectListener::Activate()
{
	Super::Activate();

	if (EffectTags1.IsEmpty())
	{
		EndTask();
		return;
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
	                      .AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectAdded);

	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::AbilitySystem_OnActiveGameplayEffectRemoved);

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
	FScopedActiveGameplayEffectLock EffectScopeLock{
		const_cast<FActiveGameplayEffectsContainer&>(AbilitySystemComponent->GetActiveGameplayEffects())
	};

	for (const auto& ActiveEffect : &AbilitySystemComponent->GetActiveGameplayEffects())
	{
		if (ActiveEffect.Spec.CapturedSourceTags.GetSpecTags().HasAny(EffectTags1))
		{
			OnEffectAdded.Broadcast(ActiveEffect.Handle);
		}
	}
}

void UFuAbilityTask_EffectListener::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_EffectListener::AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                              const FGameplayEffectSpec& EffectSpecification,
                                                                              const FActiveGameplayEffectHandle EffectHandle) const
{
	if (ShouldBroadcastAbilityTaskDelegates() && EffectSpecification.CapturedSourceTags.GetSpecTags().HasAny(EffectTags1))
	{
		OnEffectAdded.Broadcast(EffectHandle);
	}
}

void UFuAbilityTask_EffectListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	if (ShouldBroadcastAbilityTaskDelegates() && ActiveEffect.Spec.CapturedSourceTags.GetSpecTags().HasAny(EffectTags1))
	{
		OnEffectRemoved.Broadcast(ActiveEffect.Handle);
	}
}
