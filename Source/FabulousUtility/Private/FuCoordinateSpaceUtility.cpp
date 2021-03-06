#include "FuCoordinateSpaceUtility.h"

#include "SceneView.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Slate/SGameLayerManager.h"

bool UFuCoordinateSpaceUtility::TryTransformWorldToClipLocation(const APlayerController* Player,
                                                                const FVector& WorldLocation, FVector4& ClipLocation)
{
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
	FVector4 ClipLocation;
	if (!TryTransformWorldToClipLocation(Player, WorldLocation, ClipLocation))
	{
		ScreenLocation = FVector2D::ZeroVector;
		return false;
	}

	const auto Scale{
		FMath::IsNearlyZero(ClipLocation.W)
			? 1.0f / SMALL_NUMBER
			: 1.0f / FMath::Abs(ClipLocation.W)
	};

	ScreenLocation = {ClipLocation.X * Scale * 0.5f + 0.5f, 1.0f - ClipLocation.Y * Scale * 0.5f - 0.5f};
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportLocation(const APlayerController* Player,
                                                                    const FVector& WorldLocation, FVector2D& ViewportLocation)
{
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
			? 1.0f / SMALL_NUMBER
			: 1.0f / FMath::Abs(ClipLocation.W)
	};

	const FVector2D ScreenLocation{ClipLocation.X * Scale * 0.5f + 0.5f, 1.0f - ClipLocation.Y * Scale * 0.5f - 0.5f};

	const auto& ViewRect{ProjectionData.GetConstrainedViewRect()};

	ViewportLocation = {ScreenLocation.X * ViewRect.Width() + ViewRect.Min.X, ScreenLocation.Y * ViewRect.Height() + ViewRect.Min.Y};
	return true;
}

bool UFuCoordinateSpaceUtility::TryTransformWorldToViewportWidgetLocation(const APlayerController* Player, const FVector& WorldLocation,
                                                                          FVector2D& ViewportWidgetLocation)
{
	FVector2D ViewportLocation;
	if (!TryTransformWorldToViewportLocation(Player, WorldLocation, ViewportLocation))
	{
		ViewportWidgetLocation = FVector2D::ZeroVector;
		return false;
	}

	const auto* Viewport{Player->GetWorld()->GetGameViewport()};
	const auto LayerManager{IsValid(Viewport) ? Viewport->GetGameLayerManager() : nullptr};

	if (!LayerManager.IsValid())
	{
		ViewportWidgetLocation = FVector2D::ZeroVector;
		return false;
	}

	FVector2D ViewportSize;
	Viewport->GetViewportSize(ViewportSize);

	ViewportWidgetLocation = LayerManager->GetViewportWidgetHostGeometry().GetLocalSize() *
	                         ViewportLocation.RoundToVector() / ViewportSize;
	return true;
}

bool UFuCoordinateSpaceUtility::TryGetViewportWidgetSize(const UObject* WorldContext, FVector2D& ViewportWidgetSize)
{
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
