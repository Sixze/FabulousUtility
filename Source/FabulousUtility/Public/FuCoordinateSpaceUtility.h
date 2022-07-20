#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuCoordinateSpaceUtility.generated.h"

// https://docs.unrealengine.com/en-US/Basics/CoordinateSpace/

UCLASS()
class FABULOUSUTILITY_API UFuCoordinateSpaceUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToClipLocation(const APlayerController* Player, const FVector& WorldLocation, FVector4& ClipLocation);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToScreenLocation(const APlayerController* Player, const FVector& WorldLocation, FVector2D& ScreenLocation);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportLocation(const APlayerController* Player, const FVector& WorldLocation,
	                                                FVector2D& ViewportLocation);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportWidgetLocation(const APlayerController* Player, const FVector& WorldLocation,
	                                                      FVector2D& ViewportWidgetLocation);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Coordinate Space Utility", Meta = (WorldContext = "WorldContext"))
	static bool TryGetViewportWidgetSize(const UObject* WorldContext, FVector2D& ViewportWidgetSize);
};
