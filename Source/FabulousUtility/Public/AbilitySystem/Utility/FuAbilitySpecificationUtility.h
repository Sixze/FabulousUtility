#pragma once

#include "Abilities/GameplayAbility.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilitySpecificationUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilitySpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Ability Handle"))
	static FGameplayAbilitySpecHandle GetAbilityHandle(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Ability Class"))
	static TSubclassOf<UGameplayAbility> GetAbilityClass(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Input Id"))
	static uint8 GetInputId(const FGameplayAbilitySpec& AbilitySpecification);
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
