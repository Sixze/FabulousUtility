#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "FuAbilityUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilityUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility")
	static const FGameplayTagContainer& GetAbilityCooldownTags(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility")
	static TSubclassOf<UGameplayAbility> GetAbilityClass(const FGameplayAbilitySpec& AbilitySpecification);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility",
		Meta = (DefaultToSelf = "Ability", ExpandBoolAsExecs = "ReturnValue"))
	static bool IsAbilityInputPressed(const UGameplayAbility* Ability);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility")
	static bool IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility")
	static bool CanActiveAbilityByClass(UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability Utility")
	static bool CanActiveAbilityByHandle(UAbilitySystemComponent* AbilitySystem, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility")
	static bool TryBatchRpcActivateAbility(UAbilitySystemComponent* AbilitySystem,
	                                       FGameplayAbilitySpecHandle AbilityHandle,
	                                       bool bEndAbilityImmediately);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tags"))
	static void CancelAbilitiesWithAnyTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
	                                      UGameplayAbility* IgnoreAbility = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tags"))
	static void CancelAbilitiesWithoutAnyTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
	                                         UGameplayAbility* IgnoreAbility = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System", Meta = (AutoCreateRefTerm = "Tags"))
	static void RemoveAbilitiesWithAnyTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
	                                       FGameplayAbilitySpecHandle IgnoreAbilityHandle);
};

inline const FGameplayTagContainer& UFuAbilityUtility::GetAbilityCooldownTags(const TSubclassOf<UGameplayAbility> AbilityClass)
{
	const auto* Tags{FU_ENSURE(IsValid(AbilityClass)) ? AbilityClass.GetDefaultObject()->GetCooldownTags() : nullptr};

	static const FGameplayTagContainer None;
	check(None.IsEmpty())

	return Tags != nullptr ? *Tags : None;
}

inline TSubclassOf<UGameplayAbility> UFuAbilityUtility::GetAbilityClass(const FGameplayAbilitySpec& AbilitySpecification)
{
	return AbilitySpecification.Ability->GetClass();
}

inline bool UFuAbilityUtility::IsAbilityInputPressed(const UGameplayAbility* Ability)
{
	const auto* AbilitySpecification{IsValid(Ability) ? Ability->GetCurrentAbilitySpec() : nullptr};

	return FU_ENSURE(AbilitySpecification != nullptr) && AbilitySpecification->InputPressed;
}

inline bool UFuAbilityUtility::IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem,
                                                              const FGameplayAbilitySpecHandle AbilityHandle)
{
	auto bInstance{false};
	const auto* AbilityInstance{UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilitySystem, AbilityHandle, bInstance)};

	return AbilityInstance->IsActive();
}

inline bool UFuAbilityUtility::SwitchIsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystem,
                                                                    const FGameplayAbilitySpecHandle AbilityHandle)
{
	return IsPrimaryAbilityInstanceActive(AbilitySystem, AbilityHandle);
}

inline void UFuAbilityUtility::CancelAbilitiesWithAnyTag(UAbilitySystemComponent* AbilitySystem,
                                                         const FGameplayTagContainer& Tags, UGameplayAbility* IgnoreAbility)
{
	if (FU_ENSURE(IsValid(AbilitySystem)) && FU_ENSURE(Tags.IsValid()))
	{
		AbilitySystem->CancelAbilities(&Tags, nullptr, IgnoreAbility);
	}
}

inline void UFuAbilityUtility::CancelAbilitiesWithoutAnyTag(UAbilitySystemComponent* AbilitySystem,
                                                            const FGameplayTagContainer& Tags, UGameplayAbility* IgnoreAbility)
{
	if (FU_ENSURE(IsValid(AbilitySystem)) && FU_ENSURE(Tags.IsValid()))
	{
		AbilitySystem->CancelAbilities(nullptr, &Tags, IgnoreAbility);
	}
}
