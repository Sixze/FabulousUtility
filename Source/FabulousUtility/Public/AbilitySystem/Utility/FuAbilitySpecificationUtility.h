#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "FuAbilitySpecificationUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilitySpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility")
	static const TArray<FGameplayAbilitySpec>& GetActivatableAbilities(const UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility")
	static uint8 GetInputId(const FGameplayAbilitySpec& AbilitySpecification);
};

inline const TArray<FGameplayAbilitySpec>& UFuAbilitySpecificationUtility::GetActivatableAbilities(
	const UAbilitySystemComponent* AbilitySystem)
{
	static const TArray<FGameplayAbilitySpec> None;
	check(None.IsEmpty())

	return FU_ENSURE(IsValid(AbilitySystem)) ? AbilitySystem->GetActivatableAbilities() : None;
}

inline uint8 UFuAbilitySpecificationUtility::GetInputId(const FGameplayAbilitySpec& AbilitySpecification)
{
	return static_cast<uint8>(AbilitySpecification.InputID);
}
