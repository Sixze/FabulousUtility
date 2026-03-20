#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuCoordinateSpaceUtility.generated.h"

// https://dev.epicgames.com/documentation/unreal-engine/coordinate-space-terminology-in-unreal-engine?application_version=5.3

class ULocalPlayer;

UCLASS()
class FABULOUSUTILITY_API UFuCoordinateSpaceUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Clip Location (Local Player)",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToClipLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
	                                                       const FVector& WorldLocation, FVector4& ClipLocation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Clip Location (Player Controller)",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToClipLocation(const APlayerController* Player, const FVector& WorldLocation, FVector4& ClipLocation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Screen Location (Local Player)",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToScreenLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
	                                                         const FVector& WorldLocation, FVector2f& ScreenLocation,
	                                                         bool bCalculateOutsideScreenLocation = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Screen Location (Player Controller)",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToScreenLocation(const APlayerController* Player, const FVector& WorldLocation,
	                                              FVector2f& ScreenLocation, bool bCalculateOutsideScreenLocation = true);

	// Same as UGameplayStatics::ProjectWorldToScreen() but simplified.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Viewport Location (Local Player)",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportLocationLocalPlayer(const ULocalPlayer* LocalPlayer, const FVector& WorldLocation,
	                                                           FVector2f& ViewportLocation, bool bPlayerViewportRelative = false,
	                                                           bool bCalculateOutsideScreenLocation = true);

	// Same as UGameplayStatics::ProjectWorldToScreen() but simplified.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Viewport Location (Player Controller)",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportLocation(const APlayerController* Player, const FVector& WorldLocation,
	                                                FVector2f& ViewportLocation, bool bPlayerViewportRelative = false,
	                                                bool bCalculateOutsideScreenLocation = true);

	/// Useful in conjunction with Dear ImGui. To get the final location in
	/// Dear ImGui coordinate space, you can use the following code snippet:
	/// @code
	/// const auto* Viewport{ImGui::GetMainViewport()};
	///	const auto Position{NormalizedViewportLocation * Viewport->Size + Viewport->Pos};
	/// @endcode
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Normalized Viewport Location (Local Player)",
		Meta = (xpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToNormalizedViewportLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
	                                                                     const FVector& WorldLocation,
	                                                                     FVector2f& NormalizedViewportLocation,
	                                                                     bool bPlayerViewportRelative = false,
	                                                                     bool bCalculateOutsideScreenLocation = true);

	/// Useful in conjunction with Dear ImGui. To get the final location in
	/// Dear ImGui coordinate space, you can use the following code snippet:
	/// @code
	/// const auto* Viewport{ImGui::GetMainViewport()};
	///	const auto Position{FVector2f{Viewport->Pos} + NormalizedViewportLocation * FVector2f{Viewport->Size}};
	/// @endcode
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Normalized Viewport Location (Player Controller)",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToNormalizedViewportLocation(const APlayerController* Player,
	                                                          const FVector& WorldLocation,
	                                                          FVector2f& NormalizedViewportLocation,
	                                                          bool bPlayerViewportRelative = false,
	                                                          bool bCalculateOutsideScreenLocation = true);

	// Same as UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition() or UGameplayStatics::ProjectWorldToScreen()
	// with USlateBlueprintLibrary::ScreenToViewport(), but simplified and without rounding.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Viewport Widget Location (Local Player)",
		Meta = (xpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportWidgetLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
	                                                                 const FVector& WorldLocation,
	                                                                 FVector2f& ViewportWidgetLocation,
	                                                                 bool bPlayerViewportRelative = false,
	                                                                 bool bCalculateOutsideScreenLocation = true);

	// Same as UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition() or UGameplayStatics::ProjectWorldToScreen()
	// with USlateBlueprintLibrary::ScreenToViewport(), but simplified and without rounding.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		DisplayName = "Try Transform World to Viewport Widget Location (Player Controller)",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "WorldLocation"))
	static bool TryTransformWorldToViewportWidgetLocation(const APlayerController* Player,
	                                                      const FVector& WorldLocation,
	                                                      FVector2f& ViewportWidgetLocation,
	                                                      bool bPlayerViewportRelative = false,
	                                                      bool bCalculateOutsideScreenLocation = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Coordinate Space Utility",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetViewportWidgetSize(const UObject* WorldContext, FVector2f& ViewportWidgetSize);
};
