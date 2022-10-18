#include "AbilitySystem/AbilityTasks/FuAbilityTask_EffectListener.h"

#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

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

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(AbilitySystemComponent)};

	if (!FU_ENSURE(IsValid(AbilitySystem)) || EffectTags1.IsEmpty())
	{
		EndTask();
		return;
	}

	AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveGameplayEffectAdded);
	AbilitySystem->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnActiveGameplayEffectRemoved);

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	FScopedActiveGameplayEffectLock EffectScopeLock{AbilitySystem->GetActiveEffects()};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveEffects())
	{
		if (ActiveEffect.Spec.CapturedSourceTags.GetSpecTags().HasAny(EffectTags1))
		{
			OnEffectAdded.Broadcast(ActiveEffect.Handle);
		}
	}
}

void UFuAbilityTask_EffectListener::OnDestroy(const bool bInOwnerFinished)
{
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(AbilitySystemComponent)};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		AbilitySystem->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_EffectListener::OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
                                                                const FGameplayEffectSpec& EffectSpecification,
                                                                const FActiveGameplayEffectHandle EffectHandle) const
{
	if (ShouldBroadcastAbilityTaskDelegates() && EffectSpecification.CapturedSourceTags.GetSpecTags().HasAny(EffectTags1))
	{
		OnEffectAdded.Broadcast(EffectHandle);
	}
}

void UFuAbilityTask_EffectListener::OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const
{
	if (ShouldBroadcastAbilityTaskDelegates() && ActiveEffect.Spec.CapturedSourceTags.GetSpecTags().HasAny(EffectTags1))
	{
		OnEffectRemoved.Broadcast(ActiveEffect.Handle);
	}
}
