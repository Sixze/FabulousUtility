#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuGameplayTagUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuGameplayTagUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Gameplay Tag Utility", Meta = (AutoCreateRefTerm = "Tag"))
	static FName GetSimpleTagName(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Gameplay Tag Utility", Meta = (AutoCreateRefTerm = "Tag"))
	static bool HasChildTags(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Gameplay Tag Utility", Meta = (AutoCreateRefTerm = "Tag"))
	static FGameplayTagContainer GetChildTagsWithoutDescendants(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Gameplay Tag Utility", Meta = (AutoCreateRefTerm = "ParentTag"))
	static FGameplayTag GetFirstDescendantTag(const FGameplayTagContainer& Tags, const FGameplayTag& ParentTag);
};
