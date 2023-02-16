#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuCoordinateSpaceUtility.generated.h"

// https://docs.unrealengine.com/coordinate-space-terminology-in-unreal-engine/

UCLASS()
class FABULOUSUTILITY_API UFuCoordinateSpaceUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToClipLocation(const APlayerController* Player, const FVector& WorldLocation, FVector4& ClipLocation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToScreenLocation(const APlayerController* Player, const FVector& WorldLocation, FVector2D& ScreenLocation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportLocation(const APlayerController* Player, const FVector& WorldLocation,
	                                                FVector2D& ViewportLocation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportWidgetLocation(const APlayerController* Player, const FVector& WorldLocation,
	                                                      FVector2D& ViewportWidgetLocation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Coordinate Space Utility",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetViewportWidgetSize(const UObject* WorldContext, FVector2D& ViewportWidgetSize);
};
