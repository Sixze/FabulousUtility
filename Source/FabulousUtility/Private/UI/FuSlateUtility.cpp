#include "UI/FuSlateUtility.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/UserInterfaceSettings.h"

void UFuSlateUtility::PlaySound(const FSlateSound& Sound)
{
#if !UE_SERVER
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().PlaySound(Sound);
	}
#endif
}

float UFuSlateUtility::GetDpiScale(const FVector2D& WidgetSize)
{
	const auto* UserInterfaceSettings{GetDefault<UUserInterfaceSettings>()};

	if (!WidgetSize.IsNearlyZero())
	{
		return UserInterfaceSettings->GetDPIScaleBasedOnSize({
			FMath::RoundToInt32(WidgetSize.X),
			FMath::RoundToInt32(WidgetSize.Y)
		});
	}

	if (IsValid(GEngine))
	{
		const auto* World{GEngine->GetWorld()};
		const auto* Viewport{IsValid(World) ? World->GetGameViewport() : GEngine->GameViewport.Get()};

		if (IsValid(Viewport))
		{
			FVector2D ViewportSize;
			Viewport->GetViewportSize(ViewportSize);

			const auto ViewportDpiScaleInverse{1.0f / Viewport->GetDPIScale()};

			return UserInterfaceSettings->GetDPIScaleBasedOnSize({
				FMath::RoundToInt32(ViewportSize.X * ViewportDpiScaleInverse),
				FMath::RoundToInt32(ViewportSize.Y * ViewportDpiScaleInverse)
			});
		}
	}

	return UserInterfaceSettings->GetDPIScaleBasedOnSize(FIntPoint::ZeroValue);
}
