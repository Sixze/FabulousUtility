#include "AbilityAsync/FuAbilityAsync_AbilityFailureListener.h"

#include "AbilitySystemGlobals.h"
#include "FuAbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_AbilityFailureListener)

UFuAbilityAsync_AbilityFailureListener* UFuAbilityAsync_AbilityFailureListener::FuListenForAbilityFailureOnActor(
	const AActor* Actor, const FGameplayTag InAbilityTag, const FGameplayTagContainer InFailureTags)
{
	return FuListenForAbilityFailure(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor), InAbilityTag, InFailureTags);
}

UFuAbilityAsync_AbilityFailureListener* UFuAbilityAsync_AbilityFailureListener::FuListenForAbilitiesFailureOnActor(
	const AActor* Actor, const FGameplayTagContainer InAbilityTags, const FGameplayTagContainer InFailureTags)
{
	return FuListenForAbilitiesFailure(UFuAbilitySystemComponent::GetFuAbilitySystem(Actor), InAbilityTags, InFailureTags);
}

UFuAbilityAsync_AbilityFailureListener* UFuAbilityAsync_AbilityFailureListener::FuListenForAbilityFailure(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag InAbilityTag, const FGameplayTagContainer InFailureTags)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (InAbilityTag.IsValid())
	{
		Task->AbilityTags.AddTag(InAbilityTag);
	}

	for (const auto& Tag : InFailureTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->FailureTags.AddTag(Tag);
		}
	}

	return Task;
}

UFuAbilityAsync_AbilityFailureListener* UFuAbilityAsync_AbilityFailureListener::FuListenForAbilitiesFailure(
	UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer InAbilityTags, const FGameplayTagContainer InFailureTags)
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

	for (const auto& Tag : InFailureTags)
	{
		if (FU_ENSURE(Tag.IsValid()))
		{
			Task->FailureTags.AddTag(Tag);
		}
	}

	return Task;
}

void UFuAbilityAsync_AbilityFailureListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};

	if (!IsValid(GetAbilitySystemComponent()) || !FU_ENSURE(IsValid(AbilitySystem)))
	{
		EndAction();
		return;
	}

	AbilitySystem->OnAbilityFailed.AddUObject(this, &ThisClass::AbilitySystem_OnAbilityFailed);
}

void UFuAbilityAsync_AbilityFailureListener::EndAction()
{
	auto* AbilitySystem{Cast<UFuAbilitySystemComponent>(GetAbilitySystemComponent())};
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->OnAbilityFailed.RemoveAll(this);
	}

	Super::EndAction();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UFuAbilityAsync_AbilityFailureListener::AbilitySystem_OnAbilityFailed(const FGameplayAbilitySpecHandle AbilityHandle,
                                                                           UGameplayAbility* Ability,
                                                                           const FGameplayTagContainer& ActivationFailureTags) const
{
	const auto* AbilitySpecification{GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilityHandle)};

	if (ActivationFailureTags.HasAny(FailureTags) &&
	    (Ability->AbilityTags.HasAny(AbilityTags) ||
	     (AbilitySpecification != nullptr && AbilitySpecification->DynamicAbilityTags.HasAny(AbilityTags))))
	{
		OnAbilityFailed.Broadcast(AbilityHandle, FailureTags);
	}
}
