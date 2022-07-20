#pragma once

#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuGameplayCueUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuGameplayCueUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Gameplay Cue Utility")
	static TArray<AActor*> GetActorsFromCueParameters(const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void ExecuteLocalGameplayCueActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
	                                         const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void AddLocalGameplayCueActor(AActor* Actor, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void RemoveLocalGameplayCueActor(AActor* Actor, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void ExecuteLocalGameplayCuesActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
	                                          const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void AddLocalGameplayCuesActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
	                                      const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void RemoveLocalGameplayCuesActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
	                                         const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void ExecuteLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
	                                    const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void AddLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
	                                const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void RemoveLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
	                                   const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void ExecuteLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
	                                     const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void AddLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
	                                 const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void RemoveLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
	                                    const FGameplayCueParameters& Parameters);
};
