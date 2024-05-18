#include "AbilityAsync/FuAbilityAsync_TagListener.h"

#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_TagListener)

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::ListenForTagChangeOnActor(const AActor* Actor, const FGameplayTag InTag)
{
	return ListenForTagChange(UFuAbilitySystemUtility::GetAbilitySystem(Actor), InTag);
}

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::ListenForTagsChangeOnActor(const AActor* Actor,
                                                                                     const FGameplayTagContainer InTags)
{
	return ListenForTagsChange(UFuAbilitySystemUtility::GetAbilitySystem(Actor), InTags);
}

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::ListenForTagChange(UAbilitySystemComponent* AbilitySystem,
                                                                             const FGameplayTag InTag)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(InTag.IsValid()))
	{
		Task->Tags.AddTag(InTag);
	}

	return Task;
}

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::ListenForTagsChange(UAbilitySystemComponent* AbilitySystem,
                                                                              const FGameplayTagContainer InTags)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : InTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->Tags.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityAsync_TagListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || Tags.IsEmpty())
	{
		EndAction();
		return;
	}

	for (const auto& Tag : Tags)
	{
		AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
		             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);
	}

	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	for (const auto& Tag : Tags)
	{
		if (AbilitySystem->GetTagCount(Tag) > 0)
		{
			OnTagAdded.Broadcast(Tag);
		}
		else
		{
			OnTagRemoved.Broadcast(Tag);
		}
	}
}

void UFuAbilityAsync_TagListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		for (const auto& Tag : Tags)
		{
			AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_TagListener::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 TagCount) const
{
	if (ShouldBroadcastDelegates())
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
