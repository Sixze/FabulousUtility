#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuIntersectionUtility.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuIntersectionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Intersection Utility", Meta = (ReturnDisplayName = "Value"))
	static bool DoSegmentsIntersect1D(float SegmentStartA, float SegmentEndA, float SegmentStartB, float SegmentEndB);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Intersection Utility", Meta = (ReturnDisplayName = "Success"))
	static bool LineSegmentIntersection2D(FVector2D LineOrigin, FVector2D LineDirection,
	                                      FVector2D SegmentStart, FVector2D SegmentEnd, FVector2D& Intersection);
};
