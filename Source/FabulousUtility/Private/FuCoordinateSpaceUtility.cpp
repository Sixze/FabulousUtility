#include "FuCoordinateSpaceUtility.h"

#include "SceneView.h"
#include "UnrealClient.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Slate/SGameLayerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuCoordinateSpaceUtility)

bool UFuCoordinateSpaceUtility::TryTransformWorldToClipLocation(const APlayerController* Player,
                                                                const FVector& WorldLocation, FVector4& ClipLocation)
{
	// Based on APlayerController::ProjectWorldLocationToScreenWithDistance() and FSceneView::ProjectWorldToScreen().

	const auto* LocalPlayer{IsValid(Player) ? Player->GetLocalPlayer() : nullptr};
	const auto* ViewportClient{IsValid(LocalPlayer) ? LocalPlayer->ViewportClient.Get() : nullptr};

	if (!IsValid(ViewportClient))
	{
		ClipLocation = FVector4{};
		return false;
	}

	FSceneViewProjectionData ProjectionData;
	if (!LocalPlayer->GetProjectionData(ViewportClient->Viewport, ProjectionData))
	{
		ClipLocation = FVector4{};
		return false;
	}

	const auto ProjectionMatrix{ProjectionData.ComputeViewProjectionMatrix()};

	ClipLocation = ProjectionMatrix.TransformFVector4({WorldLocation, 1.0f});
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToScreenLocation(const APlayerController* Player,
                                                                  const FVector& WorldLocation, FVector2D& ScreenLocation)
{
	// Based on SceneView::ProjectWorldToScreen().

	FVector4 ClipLocation;
	if (!TryTransformWorldToClipLocation(Player, WorldLocation, ClipLocation))
	{
		ScreenLocation = FVector2D::ZeroVector;
		return false;
	}

	const auto Scale{
		FMath::IsNearlyZero(ClipLocation.W)
			? 1.0f / UE_SMALL_NUMBER
			: 1.0f / FMath::Abs(ClipLocation.W)
	};

	ScreenLocation = {ClipLocation.X * Scale, ClipLocation.Y * Scale};
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportLocation(const APlayerController* Player,
                                                                    const FVector& WorldLocation, FVector2D& ViewportLocation)
{
	// Based on APlayerController::ProjectWorldLocationToScreenWithDistance() and FSceneView::ProjectWorldToScreen().

	const auto* LocalPlayer{IsValid(Player) ? Player->GetLocalPlayer() : nullptr};
	const auto* ViewportClient{IsValid(LocalPlayer) ? LocalPlayer->ViewportClient.Get() : nullptr};

	if (!IsValid(ViewportClient))
	{
		ViewportLocation = FVector2D::ZeroVector;
		return false;
	}

	FSceneViewProjectionData ProjectionData;
	if (!LocalPlayer->GetProjectionData(ViewportClient->Viewport, ProjectionData))
	{
		ViewportLocation = FVector2D::ZeroVector;
		return false;
	}

	const auto ProjectionMatrix{ProjectionData.ComputeViewProjectionMatrix()};
	const auto ClipLocation{ProjectionMatrix.TransformFVector4({WorldLocation, 1.0})};

	const auto Scale{
		FMath::IsNearlyZero(ClipLocation.W)
			? 1.0f / UE_SMALL_NUMBER
			: 1.0f / FMath::Abs(ClipLocation.W)
	};

	const FVector2D ScreenLocation{ClipLocation.X * Scale, ClipLocation.Y * Scale};
	const auto& ViewRect{ProjectionData.GetConstrainedViewRect()};

	ViewportLocation = {
		(ScreenLocation.X * 0.5f + 0.5f) * ViewRect.Width() + ViewRect.Min.X,
		(1.0f - ScreenLocation.Y * 0.5f - 0.5f) * ViewRect.Height() + ViewRect.Min.Y
	};

	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportWidgetLocation(const APlayerController* Player, const FVector& WorldLocation,
                                                                          FVector2D& ViewportWidgetLocation)
{
	// Based on USlateBlueprintLibrary::ScreenToWidgetAbsolute().

	FVector2D ViewportLocation;
	if (!TryTransformWorldToViewportLocation(Player, WorldLocation, ViewportLocation))
	{
		ViewportWidgetLocation = FVector2D::ZeroVector;
		return false;
	}

	const auto* ViewportClient{Player->GetLocalPlayer()->ViewportClient.Get()};

	const auto ViewportSize{ViewportClient->Viewport->GetSizeXY()};
	if (ViewportSize.X == 0 || ViewportSize.Y == 0)
	{
		ViewportWidgetLocation = FVector2D::ZeroVector;
		return false;
	}

	const auto LayerManager{ViewportClient->GetGameLayerManager()};
	if (!LayerManager.IsValid())
	{
		ViewportWidgetLocation = FVector2D::ZeroVector;
		return false;
	}

	ViewportWidgetLocation = LayerManager->GetViewportWidgetHostGeometry().GetLocalSize() * ViewportLocation / ViewportSize;
	return true;
}

bool UFuCoordinateSpaceUtility::TryGetViewportWidgetSize(const UObject* WorldContext, FVector2D& ViewportWidgetSize)
{
	// Based on UWidgetLayoutLibrary::GetViewportWidgetGeometry().

	const auto* World{WorldContext->GetWorld()};
	const auto* Viewport{IsValid(World) ? World->GetGameViewport() : nullptr};
	const auto LayerManager{IsValid(Viewport) ? Viewport->GetGameLayerManager() : nullptr};

	if (!LayerManager.IsValid())
	{
		ViewportWidgetSize = FVector2D::ZeroVector;
		return false;
	}

	ViewportWidgetSize = LayerManager->GetViewportWidgetHostGeometry().GetLocalSize();
	return true;
}
