#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectSpecificationUtility.generated.h"

struct FGameplayEffectSpec;
struct FGameplayTag;
struct FGameplayTagContainer;
struct FGameplayEffectSpecHandle;

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectSpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility", Meta = (ReturnDisplayName = "Value"))
	static bool HasAssetTag(const FGameplayEffectSpec& EffectSpecification, FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Value"))
	static bool HasAnyAssetTags(const FGameplayEffectSpec& EffectSpecification, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility", Meta = (ReturnDisplayName = "Value"))
	static bool HasGrantedTag(const FGameplayEffectSpec& EffectSpecification, FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Value"))
	static bool HasAnyGrantedTags(const FGameplayEffectSpec& EffectSpecification, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (GameplayTagFilter = "SetByCaller", ReturnDisplayName = "Magnitude"))
	static float FindSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectHandle, FGameplayTag SetByCallerTag);
};
