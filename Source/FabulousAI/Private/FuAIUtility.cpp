#include "FuAIUtility.h"

#include "NavigationSystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAIUtility)

bool UFuAIUtility::TryProjectLocationToNavigation(UObject* WorldContext, const FVector& Location, ANavigationData* NavigationData,
                                                  const TSubclassOf<UNavigationQueryFilter> QueryFilterClass,
                                                  const FVector& QueryExtent, FVector& ResultLocation)
{
	if (!IsValid(NavigationData))
	{
		auto* NavigationSystem{FNavigationSystem::GetCurrent<UNavigationSystemV1>(WorldContext->GetWorld())};
		if (IsValid(NavigationSystem))
		{
			NavigationData = NavigationSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
		}
	}

	FNavLocation NavigationLocation;

	if (!IsValid(NavigationData) ||
	    !NavigationData->ProjectPoint(Location, NavigationLocation,
	                                  QueryExtent.IsNearlyZero() ? NavigationData->GetConfig().DefaultQueryExtent : QueryExtent,
	                                  UNavigationQueryFilter::GetQueryFilter(*NavigationData, WorldContext, QueryFilterClass)))
	{
		ResultLocation = Location;
		return false;
	}

	ResultLocation = NavigationLocation.Location;
	return true;
}
