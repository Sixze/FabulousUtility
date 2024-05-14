#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectSpecificationUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectSpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Value"))
	static bool HasAssetTag(const FGameplayEffectSpec& EffectSpecification, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Value"))
	static bool HasAnyAssetTags(const FGameplayEffectSpec& EffectSpecification, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Value"))
	static bool HasGrantedTag(const FGameplayEffectSpec& EffectSpecification, const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Value"))
	static bool HasAnyGrantedTags(const FGameplayEffectSpec& EffectSpecification, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "SetByCallerTag", GameplayTagFilter = "SetByCaller", ReturnDisplayName = "Magnitude"))
	static float FindSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectHandle, const FGameplayTag& SetByCallerTag);
};
