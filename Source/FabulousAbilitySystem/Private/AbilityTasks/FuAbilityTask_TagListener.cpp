#include "AbilityTasks/FuAbilityTask_TagListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_TagListener)

UFuAbilityTask_TagListener* UFuAbilityTask_TagListener::WaitForTagChange(UGameplayAbility* OwningAbility, const FGameplayTag InTag)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(InTag.IsValid()))
	{
		Task->Tags.AddTag(InTag);
	}

	return Task;
}

UFuAbilityTask_TagListener* UFuAbilityTask_TagListener::WaitForTagsChange(UGameplayAbility* OwningAbility,
                                                                          const FGameplayTagContainer InTags)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	for (const auto& Tag : InTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->Tags.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityTask_TagListener::Activate()
{
	Super::Activate();

	if (Tags.IsEmpty())
	{
		EndTask();
		return;
	}

	for (const auto& Tag : Tags)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
		                      .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);
	}

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	for (const auto& Tag : Tags)
	{
		if (AbilitySystemComponent->GetTagCount(Tag) > 0)
		{
			OnTagAdded.Broadcast(Tag);
		}
		else
		{
			OnTagRemoved.Broadcast(Tag);
		}
	}
}

void UFuAbilityTask_TagListener::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		for (const auto& Tag : Tags)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_TagListener::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 TagCount) const
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (TagCount > 0)
		{
			OnTagAdded.Broadcast(Tag);
		}
		else
		{
			OnTagRemoved.Broadcast(Tag);
		}
	}
}
