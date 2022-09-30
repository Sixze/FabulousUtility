#pragma once

#include "GameplayEffect.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectUtility.generated.h"

class UFuAbilitySystemComponent;

UCLASS()
class FABULOUSUTILITY_API UFuEffectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Effect

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility")
	static const FGameplayTagContainer& GetEffectOwnedTags(TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility")
	static int32 GetEffectStackCountByClass(const UFuAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectQuery"))
	static bool HasAnyActiveEffectsByQuery(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectQuery"))
	static void GetActiveEffectsByQuery(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery,
	                                    TArray<FActiveGameplayEffect>& ActiveEffects);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectTag"))
	static bool HasAnyActiveEffectsWithTag(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag,
	                                       bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectTags"))
	static bool HasAnyActiveEffectsWithAnyTags(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& EffectTags,
	                                           bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectTag"))
	static int32 GetEffectsCountWithTag(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag,
	                                    bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectTags"))
	static int32 GetEffectsCountWithAnyTags(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& EffectTags,
	                                        bool bIgnoreInhibitedEffects = false);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility", Meta = (AutoCreateRefTerm = "EffectTag"))
	static void GetEffectTimeRemainingAndDurationByTag(const UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag,
	                                                   float& TimeRemaining, float& Duration);

	static const FActiveGameplayEffect* GetActiveEffectTimeRemainingAndDurationByTag(
		const UFuAbilitySystemComponent* AbilitySystem, const FGameplayTag& EffectTag, float& TimeRemaining, float& Duration);

	// Effect Handle

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Effect Utility")
	static bool IsEffectActive(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsEffectActive(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility")
	static void RemoveActiveEffect(FActiveGameplayEffectHandle EffectHandle, int32 StacksToRemove = -1);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility")
	static void RecalculateEffectModifiers(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility")
	static void SetEffectDuration(FActiveGameplayEffectHandle EffectHandle, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility")
	static void SetEffectTimeRemaining(FActiveGameplayEffectHandle EffectHandle, float TimeRemaining);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Utility")
	static void IncreaseEffectTimeRemaining(FActiveGameplayEffectHandle EffectHandle, float AdditionalTimeRemaining);
};
