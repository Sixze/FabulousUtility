#include "FuCoordinateSpaceUtility.h"

#include "SceneView.h"
#include "UnrealClient.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Slate/SGameLayerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuCoordinateSpaceUtility)

bool UFuCoordinateSpaceUtility::TryTransformWorldToClipLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
                                                                           const FVector& WorldLocation, FVector4& ClipLocation)
{
	// Based on APlayerController::ProjectWorldLocationToScreenWithDistance() and FSceneView::ProjectWorldToScreen().

	const auto* Viewport{IsValid(LocalPlayer) ? LocalPlayer->ViewportClient.Get() : nullptr};
	if (!IsValid(Viewport))
	{
		ClipLocation = FVector4{};
		return false;
	}

	FSceneViewProjectionData ProjectionData;
	if (!LocalPlayer->GetProjectionData(Viewport->Viewport, ProjectionData))
	{
		ClipLocation = FVector4{};
		return false;
	}

	const auto ProjectionMatrix{ProjectionData.ComputeViewProjectionMatrix()};

	ClipLocation = ProjectionMatrix.TransformFVector4({WorldLocation, 1.0f});
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToClipLocation(const APlayerController* Player,
                                                                const FVector& WorldLocation, FVector4& ClipLocation)
{
	if (!IsValid(Player))
	{
		ClipLocation = FVector4{};
		return false;
	}

	return TryTransformWorldToClipLocationLocalPlayer(Player->GetLocalPlayer(), WorldLocation, ClipLocation);
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToScreenLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
                                                                             const FVector& WorldLocation, FVector2f& ScreenLocation)
{
	// Based on SceneView::ProjectWorldToScreen().

	FVector4 ClipLocation;
	if (!TryTransformWorldToClipLocationLocalPlayer(LocalPlayer, WorldLocation, ClipLocation))
	{
		ScreenLocation = FVector2f::ZeroVector;
		return false;
	}

	const auto Scale{
		FMath::IsNearlyZero(ClipLocation.W)
			? 1.0f / UE_SMALL_NUMBER
			: 1.0f / FMath::Abs(ClipLocation.W)
	};

	ScreenLocation = {UE_REAL_TO_FLOAT(ClipLocation.X * Scale), UE_REAL_TO_FLOAT(ClipLocation.Y * Scale)};
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToScreenLocation(const APlayerController* Player,
                                                                  const FVector& WorldLocation, FVector2f& ScreenLocation)
{
	if (!IsValid(Player))
	{
		ScreenLocation = FVector2f::ZeroVector;
		return false;
	}

	return TryTransformWorldToScreenLocationLocalPlayer(Player->GetLocalPlayer(), WorldLocation, ScreenLocation);
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
                                                                               const FVector& WorldLocation, FVector2f& ViewportLocation,
                                                                               const bool bPlayerViewportRelative)
{
	// Based on APlayerController::ProjectWorldLocationToScreenWithDistance() and FSceneView::ProjectWorldToScreen().

	const auto* Viewport{IsValid(LocalPlayer) ? LocalPlayer->ViewportClient.Get() : nullptr};
	if (!IsValid(Viewport))
	{
		ViewportLocation = FVector2f::ZeroVector;
		return false;
	}

	FSceneViewProjectionData ProjectionData;
	if (!LocalPlayer->GetProjectionData(Viewport->Viewport, ProjectionData))
	{
		ViewportLocation = FVector2f::ZeroVector;
		return false;
	}

	const auto ProjectionMatrix{ProjectionData.ComputeViewProjectionMatrix()};
	const auto ClipLocation{ProjectionMatrix.TransformFVector4({WorldLocation, 1.0f})};

	const auto Scale{
		FMath::IsNearlyZero(ClipLocation.W)
			? 1.0f / UE_SMALL_NUMBER
			: 1.0f / FMath::Abs(ClipLocation.W)
	};

	const FVector2f ScreenLocation{UE_REAL_TO_FLOAT(ClipLocation.X * Scale), UE_REAL_TO_FLOAT(ClipLocation.Y * Scale)};
	const auto& ViewRect{ProjectionData.GetConstrainedViewRect()};

	ViewportLocation = {
		(ScreenLocation.X * 0.5f + 0.5f) * ViewRect.Width(),
		(1.0f - ScreenLocation.Y * 0.5f - 0.5f) * ViewRect.Height()
	};

	if (!bPlayerViewportRelative)
	{
		ViewportLocation += ViewRect.Min;
	}

	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportLocation(const APlayerController* Player, const FVector& WorldLocation,
                                                                    FVector2f& ViewportLocation, const bool bPlayerViewportRelative)
{
	if (!IsValid(Player))
	{
		ViewportLocation = FVector2f::ZeroVector;
		return false;
	}

	return TryTransformWorldToViewportLocationLocalPlayer(Player->GetLocalPlayer(), WorldLocation, ViewportLocation,
	                                                      bPlayerViewportRelative);
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportWidgetLocationLocalPlayer(const ULocalPlayer* LocalPlayer,
                                                                                     const FVector& WorldLocation,
                                                                                     FVector2f& ViewportWidgetLocation,
                                                                                     const bool bPlayerViewportRelative)
{
	// Based on USlateBlueprintLibrary::ScreenToWidgetAbsolute().

	FVector2f ViewportLocation;
	if (!TryTransformWorldToViewportLocationLocalPlayer(LocalPlayer, WorldLocation, ViewportLocation, bPlayerViewportRelative))
	{
		ViewportWidgetLocation = FVector2f::ZeroVector;
		return false;
	}

	const auto* Viewport{LocalPlayer->ViewportClient.Get()};

	const auto ViewportSize{Viewport->Viewport->GetSizeXY()};
	if (ViewportSize.X <= 0 || ViewportSize.Y <= 0)
	{
		ViewportWidgetLocation = FVector2f::ZeroVector;
		return false;
	}

	const auto LayerManager{Viewport->GetGameLayerManager()};
	if (!LayerManager.IsValid())
	{
		ViewportWidgetLocation = FVector2f::ZeroVector;
		return false;
	}

	ViewportWidgetLocation = LayerManager->GetViewportWidgetHostGeometry().GetLocalSize() * ViewportLocation / ViewportSize;
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportWidgetLocation(const APlayerController* Player, const FVector& WorldLocation,
                                                                          FVector2f& ViewportWidgetLocation,
                                                                          const bool bPlayerViewportRelative)
{
	if (!IsValid(Player))
	{
		ViewportWidgetLocation = FVector2f::ZeroVector;
		return false;
	}

	return TryTransformWorldToViewportWidgetLocationLocalPlayer(Player->GetLocalPlayer(), WorldLocation,
	                                                            ViewportWidgetLocation, bPlayerViewportRelative);
}

bool UFuCoordinateSpaceUtility::TryGetViewportWidgetSize(const UObject* WorldContext, FVector2f& ViewportWidgetSize)
{
	// Based on UWidgetLayoutLibrary::GetViewportWidgetGeometry().

	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	const auto* Viewport{IsValid(World) ? World->GetGameViewport() : nullptr};
	const auto LayerManager{IsValid(Viewport) ? Viewport->GetGameLayerManager() : nullptr};

	if (!LayerManager.IsValid())
	{
		ViewportWidgetSize = FVector2f::ZeroVector;
		return false;
	}

	ViewportWidgetSize = LayerManager->GetViewportWidgetHostGeometry().GetLocalSize();
	return true;
}
