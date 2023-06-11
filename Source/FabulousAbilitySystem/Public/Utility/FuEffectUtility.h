#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (ReturnDisplayName = "Owned Tags"))
	static const FGameplayTagContainer& GetOwnedTags(TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (ReturnDisplayName = "Stack Count"))
	static int32 GetEffectStackCountByClass(const UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectQuery", ReturnDisplayName = "Value"))
	static bool HasActiveEffectByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectQuery"))
	static void GetActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery,
	                                    TArray<FActiveGameplayEffect>& ActiveEffects);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectTag", ReturnDisplayName = "Value"))
	static bool HasActiveEffectWithTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag,
	                                   bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectTags", ReturnDisplayName = "Value"))
	static bool HasActiveEffectWithAnyTags(const UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& EffectTags,
	                                       bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectTag", ReturnDisplayName = "Effects Count"))
	static int32 GetEffectsCountWithTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag,
	                                    bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectTags", ReturnDisplayName = "Effects Count"))
	static int32 GetEffectsCountWithAnyTags(const UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& EffectTags,
	                                        bool bIgnoreInhibitedEffects = false);

	static const FActiveGameplayEffect* GetActiveEffectTimeRemainingAndDurationByTag(
		const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag, float& TimeRemaining, float& Duration);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectTag", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
	                                                      const FGameplayTag& EffectTag, float& TimeRemaining, float& Duration);
};
