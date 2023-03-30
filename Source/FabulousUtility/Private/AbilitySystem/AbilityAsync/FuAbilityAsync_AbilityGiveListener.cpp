#include "AbilitySystem/AbilityAsync/FuAbilityAsync_AbilityGiveListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_AbilityGiveListener)

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilityGivenOnActor(
	const AActor* Actor, const FGameplayTag InAbilityTag)
{
	return FuListenForAbilityGiven(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)), InAbilityTag);
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilitiesGivenOnActor(
	const AActor* Actor, const FGameplayTagContainer InAbilityTags)
{
	return FuListenForAbilitiesGiven(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)), InAbilityTags);
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilityGiven(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag InAbilityTag)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(InAbilityTag.IsValid()))
	{
		Task->AbilityTags.AddTag(InAbilityTag);
	}

	return Task;
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilitiesGiven(
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

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)) || AbilityTags.IsEmpty())
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
		if (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
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
	    (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)))
	{
		OnAbilityGiven.Broadcast(AbilitySpecification.Handle);
	}
}

void UFuAbilityAsync_AbilityGiveListener::AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification) const
{
	if (ShouldBroadcastDelegates() &&
	    (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags)))
	{
		OnAbilityRemoved.Broadcast(AbilitySpecification.Handle);
	}
}
