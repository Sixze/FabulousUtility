#pragma once

#include "Abilities/GameplayAbility.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilitySpecificationUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilitySpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Ability Handle"))
	static FGameplayAbilitySpecHandle GetAbilityHandle(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Ability Class"))
	static TSubclassOf<UGameplayAbility> GetAbilityClass(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Input Id"))
	static uint8 GetInputId(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ReturnDisplayName = "Value"))
	static bool IsActive(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Specification Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsActive(const FGameplayAbilitySpec& AbilitySpecification);
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

inline bool UFuAbilitySpecificationUtility::SwitchIsActive(const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.IsActive();
}
