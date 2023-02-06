#include "AbilitySystem/AbilityAsync/FuAbilityAsync_TagListener.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FuMacros.h"

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::FuListenForTagChangeOnActor(const AActor* Actor, const FGameplayTag Tag)
{
	return FuListenForTagChange(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), Tag);
}

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::FuListenForTagsChangeOnActor(const AActor* Actor,
                                                                                       const FGameplayTagContainer Tags)
{
	return FuListenForTagsChange(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), Tags);
}

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::FuListenForTagChange(UAbilitySystemComponent* AbilitySystem,
                                                                               const FGameplayTag Tag)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(Tag.IsValid()))
	{
		Task->Tags1.AddTag(Tag);
	}

	return Task;
}

UFuAbilityAsync_TagListener* UFuAbilityAsync_TagListener::FuListenForTagsChange(UAbilitySystemComponent* AbilitySystem,
                                                                                const FGameplayTagContainer Tags)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : Tags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->Tags1.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityAsync_TagListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || Tags1.IsEmpty())
	{
		EndAction();
		return;
	}

	for (const auto& Tag : Tags1)
	{
		AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
		             .AddUObject(this, &ThisClass::AbilitySystem_OnTagChanged);
	}

	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	for (const auto& Tag : Tags1)
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
		for (const auto& Tag : Tags1)
		{
			AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_TagListener::AbilitySystem_OnTagChanged(const FGameplayTag Tag, const int32 Count) const
{
	if (ShouldBroadcastDelegates())
	{
		if (Count > 0)
		{
			OnTagAdded.Broadcast(Tag);
		}
		else
		{
			OnTagRemoved.Broadcast(Tag);
		}
	}
}
