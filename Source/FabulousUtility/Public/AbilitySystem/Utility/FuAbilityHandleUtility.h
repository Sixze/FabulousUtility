#pragma once

#include "FuMacros.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilityHandleUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilityHandleUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Handle Utility",
		DisplayName = "Is Valid (Ability Handle)", Meta = (CompactNodeTitle = "Is Valid ?"))
	static bool IsValid_AbilityHandle(FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Handle Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsAbilityHandleValid(FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Handle Utility",
		DisplayName = "Equal (Ability Handle)", Meta = (CompactNodeTitle = "==", Keywords = "== equal"))
	static bool EqualEqual_AbilityHandles(FGameplayAbilitySpecHandle AbilityHandleA, FGameplayAbilitySpecHandle AbilityHandleB);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Handle Utility",
		DisplayName = "Not Equal (Ability Handle)", Meta = (CompactNodeTitle = "!=", Keywords = "!= not equal"))
	static bool NotEqual_AbilityHandles(FGameplayAbilitySpecHandle AbilityHandleA, FGameplayAbilitySpecHandle AbilityHandleB);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Handle Utility")
	static FGameplayAbilitySpecHandle GetAbilityHandleFromSpecification(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Handle Utility", Meta = (DefaultToSelf = "Ability"))
	static FGameplayAbilitySpecHandle GetAbilityHandleFromAbility(const UGameplayAbility* Ability);
};

inline bool UFuAbilityHandleUtility::IsValid_AbilityHandle(const FGameplayAbilitySpecHandle AbilityHandle)
{
	return AbilityHandle.IsValid();
}

inline bool UFuAbilityHandleUtility::SwitchIsAbilityHandleValid(const FGameplayAbilitySpecHandle AbilityHandle)
{
	return AbilityHandle.IsValid();
}

inline bool UFuAbilityHandleUtility::EqualEqual_AbilityHandles(const FGameplayAbilitySpecHandle AbilityHandleA,
                                                               const FGameplayAbilitySpecHandle AbilityHandleB)
{
	return AbilityHandleA == AbilityHandleB;
}

inline bool UFuAbilityHandleUtility::NotEqual_AbilityHandles(const FGameplayAbilitySpecHandle AbilityHandleA,
                                                             const FGameplayAbilitySpecHandle AbilityHandleB)
{
	return AbilityHandleA != AbilityHandleB;
}

inline FGameplayAbilitySpecHandle UFuAbilityHandleUtility::GetAbilityHandleFromSpecification(
	const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.Handle;
}

inline FGameplayAbilitySpecHandle UFuAbilityHandleUtility::GetAbilityHandleFromAbility(const UGameplayAbility* Ability)
{
	return FU_ENSURE(IsValid(Ability)) ? Ability->GetCurrentAbilitySpecHandle() : FGameplayAbilitySpecHandle{};
}
