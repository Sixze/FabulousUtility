#include "AbilitySystem/AbilityAsync/FuAbilityAsync_AbilityFailureListener.h"

#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_AbilityFailureListener)

UFuAbilityAsync_AbilityFailureListener* UFuAbilityAsync_AbilityFailureListener::FuListenForAbilityFailureOnActor(
	const AActor* Actor, const FGameplayTag AbilityTag, const FGameplayTagContainer InFailureTags)
{
	return FuListenForAbilityFailure(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)),
		AbilityTag, InFailureTags);
}

UFuAbilityAsync_AbilityFailureListener* UFuAbilityAsync_AbilityFailureListener::FuListenForAbilitiesFailureOnActor(
	const AActor* Actor, const FGameplayTagContainer InAbilityTags, const FGameplayTagContainer InFailureTags)
{
	return FuListenForAbilitiesFailure(
		Cast<UFuAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor)),
		InAbilityTags, InFailureTags);
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

void UFuAbilityAsync_AbilityFailureListener::AbilitySystem_OnAbilityFailed(const FGameplayAbilitySpecHandle AbilityHandle,
                                                                           // ReSharper disable once CppParameterMayBeConstPtrOrRef
                                                                           UGameplayAbility* Ability,
                                                                           const FGameplayTagContainer& ActivationFailureTags) const
{
	const auto* AbilitySpecification{GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilityHandle)};

	if (ActivationFailureTags.HasAny(FailureTags) &&
	    (Ability->AbilityTags.HasAny(AbilityTags) ||
	     // ReSharper disable once CppRedundantParentheses
	     (AbilitySpecification != nullptr && AbilitySpecification->DynamicAbilityTags.HasAny(AbilityTags))))
	{
		OnAbilityFailed.Broadcast(AbilityHandle, FailureTags);
	}
}
