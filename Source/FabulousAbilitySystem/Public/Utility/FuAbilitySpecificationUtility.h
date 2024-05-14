#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilitySpecificationUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilitySpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Specification Utility",
		Meta = (ReturnDisplayName = "Ability Handle"))
	static FGameplayAbilitySpecHandle GetAbilityHandle(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Specification Utility",
		Meta = (ReturnDisplayName = "Ability Class"))
	static TSubclassOf<UGameplayAbility> GetAbilityClass(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Specification Utility", Meta = (ReturnDisplayName = "Input Id"))
	static uint8 GetInputId(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Specification Utility", Meta = (ReturnDisplayName = "Value"))
	static bool IsActive(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Specification Utility",
		DisplayName = "Is Active (Expanded)", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsActiveExpanded(const FGameplayAbilitySpec& AbilitySpecification);

	// This is not the same function as UAbilitySystemComponent::FindAbilitySpecFromClass(), because that
	// function performs a direct class comparison, while this function checks the parent-child class relationship.
	template <typename AbilityType = UGameplayAbility> requires TIsDerivedFrom<AbilityType, UGameplayAbility>::Value
	static const FGameplayAbilitySpec* FindAbilitySpecificationByClass(const UAbilitySystemComponent* AbilitySystem);
};

inline FGameplayAbilitySpecHandle UFuAbilitySpecificationUtility::GetAbilityHandle(const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.Handle;
}

inline TSubclassOf<UGameplayAbility> UFuAbilitySpecificationUtility::GetAbilityClass(const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.Ability->GetClass();
}

inline uint8 UFuAbilitySpecificationUtility::GetInputId(const FGameplayAbilitySpec& AbilitySpecification)
{
	return static_cast<uint8>(AbilitySpecification.InputID);
}

inline bool UFuAbilitySpecificationUtility::IsActive(const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.IsActive();
}

inline bool UFuAbilitySpecificationUtility::IsActiveExpanded(const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.IsActive();
}

template <typename AbilityType> requires TIsDerivedFrom<AbilityType, UGameplayAbility>::Value
const FGameplayAbilitySpec* UFuAbilitySpecificationUtility::FindAbilitySpecificationByClass(const UAbilitySystemComponent* AbilitySystem)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return nullptr;
	}

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.Ability->IsA<AbilityType>())
		{
			return &AbilitySpecification;
		}
	}

	return nullptr;
}
