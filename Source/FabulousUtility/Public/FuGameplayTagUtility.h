#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuGameplayTagUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuGameplayTagUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility", Meta = (ReturnDisplayName = "Tag Name"))
	static FName GetSimpleTagName(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility", Meta = (ReturnDisplayName = "Value"))
	static bool HasChildTags(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility", Meta = (ReturnDisplayName = "Child Tags"))
	static FGameplayTagContainer GetChildTagsWithoutDescendants(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Gameplay Tag Utility", Meta = (ReturnDisplayName = "Descendant Tag"))
	static FGameplayTag FindFirstDescendantTag(const FGameplayTagContainer& Tags, FGameplayTag Tag);
};
