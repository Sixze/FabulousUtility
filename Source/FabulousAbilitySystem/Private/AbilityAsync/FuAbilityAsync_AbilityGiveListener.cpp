#include "AbilityAsync/FuAbilityAsync_AbilityGiveListener.h"

#include "FuAbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_AbilityGiveListener)

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::ListenForAbilityGiveOnActor(
	const AActor* Actor, const FGameplayTag InAbilityTag)
{
	return ListenForAbilityGive(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor), InAbilityTag);
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::ListenForAbilitiesGiveOnActor(
	const AActor* Actor, const FGameplayTagContainer InAbilityTags)
{
	return ListenForAbilitiesGive(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor), InAbilityTags);
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::ListenForAbilityGive(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag InAbilityTag)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	if (InAbilityTag.IsValid())
	{
		Task->AbilityTags.AddTag(InAbilityTag);
	}

	return Task;
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::ListenForAbilitiesGive(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer InAbilityTags)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : InAbilityTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->AbilityTags.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityAsync_AbilityGiveListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnAbilityGiven.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityGiven);
	AbilitySystem->OnAbilityRemoved.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityRemoved);

	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilityTags.IsEmpty() ||
		    AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
		    AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags))
		{
			OnAbilityGiven.Broadcast(AbilitySpecification.Handle);
		}
	}
}

void UFuAbilityAsync_AbilityGiveListener::EndAction()
{
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnAbilityGiven.RemoveAll(this);
		AbilitySystem->OnAbilityRemoved.RemoveAll(this);
	}

	Super::EndAction();
}

void UFuAbilityAsync_AbilityGiveListener::AbilitySystem_OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpecification) const
{
	if (ShouldBroadcastDelegates() &&
	    (AbilityTags.IsEmpty() ||
	     AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)))
	{
		OnAbilityGiven.Broadcast(AbilitySpecification.Handle);
	}
}

void UFuAbilityAsync_AbilityGiveListener::AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification) const
{
	if (ShouldBroadcastDelegates() &&
	    (AbilityTags.IsEmpty() ||
	     AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)))
	{
		OnAbilityRemoved.Broadcast(AbilitySpecification.Handle);
	}
}
