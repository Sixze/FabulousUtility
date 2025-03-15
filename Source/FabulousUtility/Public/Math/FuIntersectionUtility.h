#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuIntersectionUtility.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuIntersectionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Intersection Utility",
		Meta = (AutoCreateRefTerm = "LineOrigin, LineDirection, SegmentStart, SegmentEnd", ReturnDisplayName = "Success"))
	static bool LineSegmentIntersection2D(const FVector2D& LineOrigin, const FVector2D& LineDirection,
	                                      const FVector2D& SegmentStart, const FVector2D& SegmentEnd,
	                                      FVector2D& Intersection);
};
