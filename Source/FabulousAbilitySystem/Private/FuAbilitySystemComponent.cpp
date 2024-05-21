#include "FuAbilitySystemComponent.h"

#include "AbilitySystemInterface.h"
#include "FuGameplayAbility.h"
#include "Utility/FuAbilityUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilitySystemComponent)

UFuAbilitySystemComponent* UFuAbilitySystemComponent::GetFuAbilitySystem(const UObject* Object)
{
	const auto* Accessor{Cast<IAbilitySystemInterface>(Object)};
	if (Accessor != nullptr)
	{
		auto* AbilitySystem{Cast<ThisClass>(Accessor->GetAbilitySystemComponent())};
		if (IsValid(AbilitySystem))
		{
			return AbilitySystem;
		}
	}

	const auto* Actor{Cast<AActor>(Object)};
	if (IsValid(Actor))
	{
		return Actor->FindComponentByClass<ThisClass>();
	}

	return nullptr;
}

bool UFuAbilitySystemComponent::TryGetFuAbilitySystem(const UObject* Object, ThisClass*& AbilitySystem)
{
	const auto* Accessor{Cast<IAbilitySystemInterface>(Object)};
	if (Accessor != nullptr)
	{
		AbilitySystem = Cast<ThisClass>(Accessor->GetAbilitySystemComponent());
		if (IsValid(AbilitySystem))
		{
			return true;
		}
	}

	const auto* Actor{Cast<AActor>(Object)};
	if (IsValid(Actor))
	{
		AbilitySystem = Actor->FindComponentByClass<ThisClass>();
		return IsValid(AbilitySystem);
	}

	AbilitySystem = nullptr;
	return false;
}

void UFuAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(AbilityHandle, Ability);

	OnAbilityActivated.Broadcast(AbilityHandle, Ability);
}

void UFuAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle AbilityHandle,
                                                    UGameplayAbility* Ability, const FGameplayTagContainer& FailureTags)
{
	Super::NotifyAbilityFailed(AbilityHandle, Ability, FailureTags);

	OnAbilityFailed.Broadcast(AbilityHandle, Ability, FailureTags);
}

bool UFuAbilitySystemComponent::ShouldDoServerAbilityRPCBatch() const
{
	return true;
}

void UFuAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpecification)
{
	Super::OnGiveAbility(AbilitySpecification);

	OnAbilityGiven.Broadcast(AbilitySpecification);
}

void UFuAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpecification)
{
	OnAbilityRemoved.Broadcast(AbilitySpecification);

	Super::OnRemoveAbility(AbilitySpecification);
}

bool UFuAbilitySystemComponent::AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const
{
	return Super::AreAbilityTagsBlocked(Tags) ||
	       (BlockedAbilityWithoutAllTags.GetExplicitGameplayTags().IsValid() &&
	        !Tags.HasAny(BlockedAbilityWithoutAllTags.GetExplicitGameplayTags()));
}

void UFuAbilitySystemComponent::AbilityLocalInputPressed(const int32 InputId)
{
	// Based on UAbilitySystemComponent::AbilityLocalInputPressed().

	if (IsGenericConfirmInputBound(InputId))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputId))
	{
		LocalInputCancel();
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	TArray<FGameplayAbilitySpec*> AcceptedAbilitySpecifications;

	// Update the input state of all abilities BEFORE calling any callbacks to ensure that the input state will be always correct in all
	// scenarios, such as when on input press, one ability directly activates another, that is also listening for the same input action.

	for (auto& AbilitySpecification : GetActivatableAbilities())
	{
		if (AbilitySpecification.InputID == InputId)
		{
			AbilitySpecification.InputPressed = true;

			AcceptedAbilitySpecifications.Emplace(&AbilitySpecification);
		}
	}

	for (auto* AbilitySpecification : AcceptedAbilitySpecifications)
	{
		if (AbilitySpecification->IsActive())
		{
			if (!IsOwnerActorAuthoritative() && AbilitySpecification->Ability->bReplicateInputDirectly)
			{
				ServerSetInputPressed(AbilitySpecification->Handle);
			}

			AbilitySpecInputPressed(*AbilitySpecification);

			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpecification->Handle,
			                      AbilitySpecification->ActivationInfo.GetActivationPredictionKey());
			continue;
		}

		const auto* FuAbility{Cast<UFuGameplayAbility>(AbilitySpecification->Ability)};

		if (!IsValid(FuAbility) || FuAbility->IsActivationByInputAllowed())
		{
			if (FuAbility->CanBatchActivationByInput())
			{
				UFuAbilityUtility::BatchRpcActivateAbility(this, AbilitySpecification->Handle, false);
			}
			else
			{
				TryActivateAbility(AbilitySpecification->Handle);
			}
		}
	}
}

void UFuAbilitySystemComponent::AbilityLocalInputReleased(const int32 InputId)
{
	// Based on UAbilitySystemComponent::AbilityLocalInputReleased().

	ABILITYLIST_SCOPE_LOCK();

	TArray<FGameplayAbilitySpec*> AcceptedAbilitySpecifications;

	for (auto& AbilitySpecification : GetActivatableAbilities())
	{
		if (AbilitySpecification.InputID == InputId)
		{
			AbilitySpecification.InputPressed = false;

			AcceptedAbilitySpecifications.Emplace(&AbilitySpecification);
		}
	}

	for (auto* AbilitySpecification : AcceptedAbilitySpecifications)
	{
		if (AbilitySpecification->IsActive())
		{
			if (!IsOwnerActorAuthoritative() && AbilitySpecification->Ability->bReplicateInputDirectly)
			{
				ServerSetInputReleased(AbilitySpecification->Handle);
			}

			AbilitySpecInputReleased(*AbilitySpecification);

			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpecification->Handle,
			                      AbilitySpecification->ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UFuAbilitySystemComponent::InputTagPressed(const FGameplayTag& InputTag)
{
	// Based on UAbilitySystemComponent::AbilityLocalInputPressed().

	if (!InputTag.IsValid())
	{
		return;
	}

	if (ConfirmInputTag == InputTag)
	{
		LocalInputConfirm();
		return;
	}

	if (CancelInputTag == InputTag)
	{
		LocalInputCancel();
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	TArray<FGameplayAbilitySpec*> AcceptedAbilitySpecifications;

	for (auto& AbilitySpecification : GetActivatableAbilities())
	{
		if (IsValid(AbilitySpecification.Ability) &&
		    (AbilitySpecification.DynamicAbilityTags.HasTag(InputTag) ||
		     AbilitySpecification.Ability->AbilityTags.HasTag(InputTag)))
		{
			AbilitySpecification.InputPressed = true;

			AcceptedAbilitySpecifications.Emplace(&AbilitySpecification);
		}
	}

	for (auto* AbilitySpecification : AcceptedAbilitySpecifications)
	{
		if (AbilitySpecification->IsActive())
		{
			if (!IsOwnerActorAuthoritative() && AbilitySpecification->Ability->bReplicateInputDirectly)
			{
				ServerSetInputPressed(AbilitySpecification->Handle);
			}

			AbilitySpecInputPressed(*AbilitySpecification);

			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpecification->Handle,
			                      AbilitySpecification->ActivationInfo.GetActivationPredictionKey());
			continue;
		}

		const auto* FuAbility{Cast<UFuGameplayAbility>(AbilitySpecification->Ability)};

		if (!IsValid(FuAbility) || FuAbility->IsActivationByInputAllowed())
		{
			if (FuAbility->CanBatchActivationByInput())
			{
				UFuAbilityUtility::BatchRpcActivateAbility(this, AbilitySpecification->Handle, false);
			}
			else
			{
				TryActivateAbility(AbilitySpecification->Handle);
			}
		}
	}
}

void UFuAbilitySystemComponent::InputTagReleased(const FGameplayTag& InputTag)
{
	// Based on UAbilitySystemComponent::AbilityLocalInputReleased().

	if (!InputTag.IsValid())
	{
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	TArray<FGameplayAbilitySpec*> AcceptedAbilitySpecifications;

	for (auto& AbilitySpecification : GetActivatableAbilities())
	{
		if (IsValid(AbilitySpecification.Ability) &&
		    (AbilitySpecification.DynamicAbilityTags.HasTag(InputTag) ||
		     AbilitySpecification.Ability->AbilityTags.HasTag(InputTag)))
		{
			AbilitySpecification.InputPressed = false;

			AcceptedAbilitySpecifications.Emplace(&AbilitySpecification);
		}
	}

	for (auto* AbilitySpecification : AcceptedAbilitySpecifications)
	{
		if (AbilitySpecification->IsActive())
		{
			if (!IsOwnerActorAuthoritative() && AbilitySpecification->Ability->bReplicateInputDirectly)
			{
				ServerSetInputReleased(AbilitySpecification->Handle);
			}

			AbilitySpecInputReleased(*AbilitySpecification);

			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpecification->Handle,
			                      AbilitySpecification->ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UFuAbilitySystemComponent::BlockAbilitiesWithoutAllTags(const FGameplayTagContainer& Tags)
{
	BlockedAbilityWithoutAllTags.UpdateTagCount(Tags, 1);
}

void UFuAbilitySystemComponent::UnBlockAbilitiesWithoutAllTags(const FGameplayTagContainer& Tags)
{
	BlockedAbilityWithoutAllTags.UpdateTagCount(Tags, -1);
}
