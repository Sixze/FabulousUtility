#include "AbilityTasks/FuAbilityTask_EffectListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_EffectListener)

UFuAbilityTask_EffectListener* UFuAbilityTask_EffectListener::FuWaitForEffectChangeByTag(UGameplayAbility* OwningAbility,
                                                                                         const FGameplayTag InEffectTag)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(InEffectTag.IsValid()))
	{
		Task->EffectTags.AddTag(InEffectTag);
	}

	return Task;
}

UFuAbilityTask_EffectListener* UFuAbilityTask_EffectListener::FuWaitForEffectChangeByTags(UGameplayAbility* OwningAbility,
                                                                                          const FGameplayTagContainer InEffectTags)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	for (const auto& Tag : InEffectTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->EffectTags.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityTask_EffectListener::Activate()
{
	Super::Activate();

	if (EffectTags.IsEmpty())
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

	FScopedActiveGameplayEffectLock EffectScopeLock{
		const_cast<FActiveGameplayEffectsContainer&>(AbilitySystemComponent->GetActiveGameplayEffects())
	};

	for (const auto& ActiveEffect : &AbilitySystemComponent->GetActiveGameplayEffects())
	{
		if (ActiveEffect.Spec.CapturedSourceTags.GetSpecTags().HasAny(EffectTags))
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
	if (ShouldBroadcastAbilityTaskDelegates() && EffectSpecification.CapturedSourceTags.GetSpecTags().HasAny(EffectTags))
	{
		OnEffectAdded.Broadcast(EffectHandle);
	}
}

void UFuAbilityTask_EffectListener::AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	if (ShouldBroadcastAbilityTaskDelegates() && ActiveEffect.Spec.CapturedSourceTags.GetSpecTags().HasAny(EffectTags))
	{
		OnEffectRemoved.Broadcast(ActiveEffect.Handle);
	}
}
