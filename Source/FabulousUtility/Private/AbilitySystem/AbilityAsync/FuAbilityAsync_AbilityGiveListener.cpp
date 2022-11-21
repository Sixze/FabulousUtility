#include "AbilitySystem/AbilityAsync/FuAbilityAsync_AbilityGiveListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilityGivenActor(
	const AActor* Actor, const FGameplayTag AbilityTag)
{
	return FuListenForAbilityGiven(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)), AbilityTag);
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilitiesGivenActor(
	const AActor* Actor, const FGameplayTagContainer AbilityTags)
{
	return FuListenForAbilitiesGiven(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)), AbilityTags);
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilityGiven(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag AbilityTag)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(AbilityTag.IsValid()))
	{
		Task->AbilityTags1.AddTag(AbilityTag);
	}

	return Task;
}

UFuAbilityAsync_AbilityGiveListener* UFuAbilityAsync_AbilityGiveListener::FuListenForAbilitiesGiven(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer AbilityTags)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Tag : AbilityTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->AbilityTags1.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityAsync_AbilityGiveListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)) || AbilityTags1.IsEmpty())
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
		if (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags1) ||
		    AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags1))
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
	    (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags1) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags1)))
	{
		OnAbilityGiven.Broadcast(AbilitySpecification.Handle);
	}
}

void UFuAbilityAsync_AbilityGiveListener::AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification) const
{
	if (ShouldBroadcastDelegates() &&
	    (AbilitySpecification.DynamicAbilityTags.HasAny(AbilityTags1) ||
	     AbilitySpecification.Ability->AbilityTags.HasAny(AbilityTags1)))
	{
		OnAbilityRemoved.Broadcast(AbilitySpecification.Handle);
	}
}
