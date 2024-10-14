#include "Utility/FuAbilityUtility.h"

#include "FuGameplayAbility.h"
#include "FuGameplayTagUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityUtility)

FGameplayTag UFuAbilityUtility::FindFirstDescendantAbilityTag(const UGameplayAbility* Ability, const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(Ability)) || !FU_ENSURE(Tag.IsValid()))
	{
		return FGameplayTag::EmptyTag;
	}

	const auto DescendantTag{UFuGameplayTagUtility::FindFirstDescendantTag(Ability->GetAssetTags(), Tag)};
	if (DescendantTag.IsValid())
	{
		return DescendantTag;
	}

	const auto* AbilitySpecification{IsValid(Ability) ? Ability->GetCurrentAbilitySpec() : nullptr};
	if (!FU_ENSURE(AbilitySpecification != nullptr))
	{
		return FGameplayTag::EmptyTag;
	}

	return UFuGameplayTagUtility::FindFirstDescendantTag(AbilitySpecification->GetDynamicSpecSourceTags(), Tag);
}

bool UFuAbilityUtility::HasAbilityTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle AbilityHandle,
                                      const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	return FU_ENSURE(AbilitySpecification != nullptr) &&
	       (AbilitySpecification->GetDynamicSpecSourceTags().HasTag(Tag) ||
	        AbilitySpecification->Ability->GetAssetTags().HasTag(Tag));
}

bool UFuAbilityUtility::TryGetSourceObjectCasted(const UAbilitySystemComponent* AbilitySystem,
                                                 const FGameplayAbilitySpecHandle AbilityHandle,
                                                 const TSubclassOf<UObject> SourceObjectClass, UObject*& SourceObject)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()))
	{
		SourceObject = nullptr;
		return false;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	if (AbilitySpecification == nullptr || !AbilitySpecification->SourceObject.IsValid() ||
	    !AbilitySpecification->SourceObject->IsA(SourceObjectClass))
	{
		SourceObject = nullptr;
		return false;
	}

	SourceObject = AbilitySpecification->SourceObject.Get();
	return true;
}

FGameplayTag UFuAbilityUtility::FindFirstDescendantAbilityTagByHandle(const UAbilitySystemComponent* AbilitySystem,
                                                                      const FGameplayAbilitySpecHandle AbilityHandle,
                                                                      const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()) || !FU_ENSURE(Tag.IsValid()))
	{
		return FGameplayTag::EmptyTag;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};
	if (!FU_ENSURE(AbilitySpecification != nullptr))
	{
		return FGameplayTag::EmptyTag;
	}

	const auto DescendantTag{UFuGameplayTagUtility::FindFirstDescendantTag(AbilitySpecification->GetDynamicSpecSourceTags(), Tag)};
	if (DescendantTag.IsValid())
	{
		return DescendantTag;
	}

	return UFuGameplayTagUtility::FindFirstDescendantTag(AbilitySpecification->Ability->GetAssetTags(), Tag);
}

bool UFuAbilityUtility::TryCommitAbility(UGameplayAbility* Ability, const bool bCancelOnFailure)
{
	if (!IsValid(Ability))
	{
		return false;
	}

	if (Ability->K2_CommitAbility())
	{
		return true;
	}

	if (bCancelOnFailure)
	{
		Ability->K2_CancelAbility();
	}

	return false;
}

bool UFuAbilityUtility::HasAbilitiesWithTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.GetDynamicSpecSourceTags().HasTag(Tag) ||
		    AbilitySpecification.Ability->GetAssetTags().HasTag(Tag))
		{
			return true;
		}
	}

	return false;
}

bool UFuAbilityUtility::CanActivateAbilityByTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if ((AbilitySpecification.GetDynamicSpecSourceTags().HasTag(Tag) ||
		     AbilitySpecification.Ability->GetAssetTags().HasTag(Tag)) &&
		    AbilitySpecification.Ability->CanActivateAbility(AbilitySpecification.Handle, AbilitySystem->AbilityActorInfo.Get()))
		{
			return true;
		}
	}

	return false;
}

bool UFuAbilityUtility::CanActivateAbilityByClass(const UAbilitySystemComponent* AbilitySystem,
                                                  const TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(AbilityClass)))
	{
		return false;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromClass(AbilityClass)};

	return AbilitySpecification != nullptr &&
	       AbilitySpecification->Ability->CanActivateAbility(AbilitySpecification->Handle, AbilitySystem->AbilityActorInfo.Get());
}

bool UFuAbilityUtility::CanActivateAbilityByClassSoft(const UAbilitySystemComponent* AbilitySystem,
                                                      const TSoftClassPtr<UGameplayAbility>& AbilityClassSoft)
{
	// If the ability is not loaded, then there are no granted abilities.

	const TSubclassOf<UGameplayAbility> AbilityClass{AbilityClassSoft.Get()};
	if (!FU_ENSURE(!AbilityClassSoft.IsNull()) || !IsValid(AbilityClass))
	{
		return false;
	}

	return CanActivateAbilityByClass(AbilitySystem, AbilityClass);
}

bool UFuAbilityUtility::CanActivateAbilityByHandle(const UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayAbilitySpecHandle AbilityHandle)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()))
	{
		return false;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	return AbilitySpecification != nullptr &&
	       AbilitySpecification->Ability->CanActivateAbility(AbilityHandle, AbilitySystem->AbilityActorInfo.Get());
}

bool UFuAbilityUtility::BatchRpcActivateAbility(UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle AbilityHandle,
                                                const bool bEndAbilityImmediately)
{
	// https://github.com/tranek/GASDocumentation#concepts-ga-batching

	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()))
	{
		return false;
	}

	FScopedServerAbilityRPCBatcher RpcBatcher{AbilitySystem, AbilityHandle};

	const auto bAbilityActivated{AbilitySystem->TryActivateAbility(AbilityHandle)};

	if (!bEndAbilityImmediately)
	{
		return bAbilityActivated;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	if (AbilitySpecification == nullptr || !AbilitySpecification->IsActive() ||
	    !FU_ENSURE(AbilitySpecification->Ability->IsA<UFuGameplayAbility>()))
	{
		return bAbilityActivated;
	}

	// auto* Ability{Cast<UFuGameplayAbility>(AbilitySpecification->GetPrimaryInstance())};
	// Ability->BatchRpcEndAbility();

	for (auto* Ability : AbilitySpecification->GetAbilityInstances())
	{
		if (IsValid(Ability))
		{
			Cast<UFuGameplayAbility>(Ability)->BatchRpcEndAbility();
		}
	}

	return bAbilityActivated;
}

void UFuAbilityUtility::RemoveAbilitiesWithAnyTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
                                                   const FGameplayAbilitySpecHandle IgnoreAbilityHandle)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tags.IsValid()) || !FU_ENSURE(AbilitySystem->IsOwnerActorAuthoritative()))
	{
		return;
	}

	FScopedAbilityListLock AbilityScopeLock{*AbilitySystem};

	for (auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.Handle != IgnoreAbilityHandle &&
		    (AbilitySpecification.GetDynamicSpecSourceTags().HasAny(Tags) ||
		     AbilitySpecification.Ability->GetAssetTags().HasAny(Tags)))
		{
			AbilitySystem->CancelAbilityHandle(AbilitySpecification.Handle);
			AbilitySystem->ClearAbility(AbilitySpecification.Handle);
		}
	}
}
