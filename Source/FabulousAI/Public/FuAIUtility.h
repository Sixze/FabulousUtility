#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAIUtility.generated.h"

UCLASS(DisplayName = "Fu AI Utility")
class FABULOUSAI_API UFuAIUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous AI|Utility",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "QueryExtent"))
	static bool TryProjectLocationToNavigation(UObject* WorldContext, const FVector& Location, ANavigationData* NavigationData,
	                                           TSubclassOf<UNavigationQueryFilter> QueryFilterClass,
	                                           const FVector& QueryExtent, FVector& ResultLocation);
};
