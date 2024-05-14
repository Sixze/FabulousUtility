#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectHandleUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectHandleUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Handle Utility", Meta = (ReturnDisplayName = "Value"))
	static bool IsActive(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility",
		DisplayName = "Is Active (Expanded)", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsActiveExpanded(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility")
	static void RemoveActiveEffect(FActiveGameplayEffectHandle EffectHandle, int32 StacksToRemove = -1);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility")
	static void RemoveActiveEffects(const TArray<FActiveGameplayEffectHandle>& EffectHandles, int32 StacksToRemove = -1);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility")
	static void RecalculateEffectModifiers(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetEffectTimeRemainingAndDurationByHandle(FActiveGameplayEffectHandle EffectHandle,
	                                                         float& TimeRemaining, float& Duration);

	// This function will also restart the effect timer from the beginning. If you just want to modify the remaining
	// effect time, use UFuEffectHandleUtility::SetEffectTimeRemaining() or UFuEffectHandleUtility::IncreaseEffectTimeRemaining().
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility")
	static void SetEffectDuration(FActiveGameplayEffectHandle EffectHandle, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility")
	static void SetEffectTimeRemaining(FActiveGameplayEffectHandle EffectHandle, float TimeRemaining);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Handle Utility")
	static void IncreaseEffectTimeRemaining(FActiveGameplayEffectHandle EffectHandle, float AdditionalTimeRemaining);
};
