#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuGameplayTagUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuGameplayTagUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Tag Name"))
	static FName GetSimpleTagName(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Value"))
	static bool HasChildTags(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Child Tags"))
	static FGameplayTagContainer GetChildTagsWithoutDescendants(const FGameplayTag& Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Descendant Tag"))
	static const FGameplayTag& FindFirstDescendantTag(const FGameplayTagContainer& Tags,
	                                                  const FGameplayTag& Tag);
};
