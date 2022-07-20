#include "AbilitySystem/AbilityTasks/FuAbilityTask_TagListener.h"

#include "AbilitySystem/Utility/FuAbilitySystemUtility.h"

UFuAbilityTask_TagListener* UFuAbilityTask_TagListener::FuWaitForTagChange(UGameplayAbility* OwningAbility, const FGameplayTag Tag)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(Tag.IsValid()))
	{
		Task->Tags1.AddTag(Tag);
	}

	return Task;
}

UFuAbilityTask_TagListener* UFuAbilityTask_TagListener::FuWaitForTagsChange(UGameplayAbility* OwningAbility,
                                                                            const FGameplayTagContainer Tags)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	for (const auto& Tag : Tags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->Tags1.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityTask_TagListener::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(IsValid(AbilitySystemComponent)) || Tags1.IsEmpty())
	{
		EndTask();
		return;
	}

	for (const auto& Tag : Tags1)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
		                      .AddUObject(this, &ThisClass::OnTagChanged);
	}

	for (const auto& Tag : Tags1)
	{
		OnTagChanged(Tag, AbilitySystemComponent->GetTagCount(Tag));
	}
}

void UFuAbilityTask_TagListener::OnDestroy(const bool bInOwnerFinished)
{
	if (IsValid(AbilitySystemComponent))
	{
		for (const auto& Tag : Tags1)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_TagListener::OnTagChanged(const FGameplayTag Tag, const int32 NewCount) const
{
	if (NewCount > 0)
	{
		OnTagAdded.Broadcast(Tag);
	}
	else
	{
		OnTagRemoved.Broadcast(Tag);
	}
}
