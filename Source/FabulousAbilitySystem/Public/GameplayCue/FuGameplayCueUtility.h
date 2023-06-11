#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuGameplayCueUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuGameplayCueUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Gameplay Cue Utility", Meta = (ReturnDisplayName = "Actors"))
	static TArray<AActor*> GetActors(const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void ExecuteLocalGameplayCueOnActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
	                                           const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void AddLocalGameplayCueToActor(AActor* Actor, const FGameplayTag& GameplayCueTag, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTag, Parameters", GameplayTagFilter = "GameplayCue"))
	static void RemoveLocalGameplayCueFromActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
	                                            const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void ExecuteLocalGameplayCuesOnActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
	                                            const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void AddLocalGameplayCuesToActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
	                                        const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Cue Utility",
		Meta = (AutoCreateRefTerm = "GameplayCueTags, Parameters", GameplayTagFilter = "GameplayCue"))
	static void RemoveLocalGameplayCuesFromActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
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
