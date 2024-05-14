#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility", Meta = (ReturnDisplayName = "Owned Tags"))
	static const FGameplayTagContainer& GetGrantedTags(TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility", Meta = (ReturnDisplayName = "Stack Count"))
	static int32 GetEffectStackCountByClass(const UAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectClassSoft", ReturnDisplayName = "Stack Count"))
	static int32 GetEffectStackCountByClassSoft(const UAbilitySystemComponent* AbilitySystem,
	                                            const TSoftClassPtr<UGameplayEffect>& EffectClassSoft);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "EffectQuery", ReturnDisplayName = "Value"))
	static bool HasActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility", Meta = (AutoCreateRefTerm = "EffectQuery"))
	static void FindActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery,
	                                     TArray<FActiveGameplayEffect>& ActiveEffects);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Value"))
	static bool HasActiveEffectsWithTag(const UAbilitySystemComponent* AbilitySystem,
	                                    const FGameplayTag& Tag, bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Value"))
	static bool HasActiveEffectsWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
	                                        const FGameplayTagContainer& Tags, bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Effects Count"))
	static int32 GetEffectsCountByTag(const UAbilitySystemComponent* AbilitySystem,
	                                  const FGameplayTag& Tag, bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Effects Count"))
	static int32 GetEffectsCountWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
	                                        const FGameplayTagContainer& Tags, bool bIgnoreInhibitedEffects = false);

	static const FActiveGameplayEffect* GetActiveEffectTimeRemainingAndDurationByTag(
		const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, float& TimeRemaining, float& Duration);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Utility",
		Meta = (AutoCreateRefTerm = "Tag", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
	                                                      const FGameplayTag& Tag, float& TimeRemaining, float& Duration);
};
