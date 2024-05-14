#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilityHandleUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilityHandleUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Handle Utility",
		DisplayName = "Is Valid (Ability Handle)", Meta = (CompactNodeTitle = "Is Valid ?"))
	static bool IsValid(FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Handle Utility",
		DisplayName = "Is Valid (Expanded)", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsValidExpanded(FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Handle Utility",
		DisplayName = "Equal (Ability Handle)", Meta = (CompactNodeTitle = "==", Keywords = "== equal"))
	static bool IsEqual(FGameplayAbilitySpecHandle AbilityHandleA, FGameplayAbilitySpecHandle AbilityHandleB);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability Handle Utility",
		DisplayName = "Not Equal (Ability Handle)", Meta = (CompactNodeTitle = "!=", Keywords = "!= not equal"))
	static bool IsNotEqual(FGameplayAbilitySpecHandle AbilityHandleA, FGameplayAbilitySpecHandle AbilityHandleB);
};

inline bool UFuAbilityHandleUtility::IsValid(const FGameplayAbilitySpecHandle AbilityHandle)
{
	return AbilityHandle.IsValid();
}

inline bool UFuAbilityHandleUtility::IsValidExpanded(const FGameplayAbilitySpecHandle AbilityHandle)
{
	return AbilityHandle.IsValid();
}

inline bool UFuAbilityHandleUtility::IsEqual(const FGameplayAbilitySpecHandle AbilityHandleA,
                                             const FGameplayAbilitySpecHandle AbilityHandleB)
{
	return AbilityHandleA == AbilityHandleB;
}

inline bool UFuAbilityHandleUtility::IsNotEqual(const FGameplayAbilitySpecHandle AbilityHandleA,
                                                const FGameplayAbilitySpecHandle AbilityHandleB)
{
	return AbilityHandleA != AbilityHandleB;
}
