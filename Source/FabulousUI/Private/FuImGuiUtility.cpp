#include "FuImGuiUtility.h"

#include "FuCoordinateSpaceUtility.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuImGuiUtility)

#if defined(WITH_IMGUI) && !defined(IMGUI_DISABLE)
ImVec4 UFuImGuiUtility::AdjustHueAndSaturation(const ImVec4& Color, const float HueShift, const float SaturationScale)
{
	auto ResultColor{Color};
	ImGui::ColorConvertRGBtoHSV(ResultColor.x, ResultColor.y, ResultColor.z, ResultColor.x, ResultColor.y, ResultColor.z);

	ResultColor.x += HueShift;
	ResultColor.y *= SaturationScale;

	ImGui::ColorConvertHSVtoRGB(ResultColor.x, ResultColor.y, ResultColor.z, ResultColor.x, ResultColor.y, ResultColor.z);
	return ResultColor;
}

bool UFuImGuiUtility::IsKeysPressed(const TArray<FKey>& Keys)
{
	auto bAnyKeyJustPressed{false};

	for (const auto& Key : Keys)
	{
		const auto ImGuiKey{ImGui::ConvertKey(Key)};
		const auto bKeyJustPressed{ImGui::IsKeyPressed(ImGuiKey, ImGuiInputFlags_None)};

		if (!bKeyJustPressed && !ImGui::IsKeyDown(ImGuiKey))
		{
			return false;
		}

		bAnyKeyJustPressed |= bKeyJustPressed;
	}

	return bAnyKeyJustPressed;
}

bool UFuImGuiUtility::TryTransformWorldToMainViewportPositionLocalPlayer(const ULocalPlayer* LocalPlayer, const FVector& WorldLocation,
                                                                         ImVec2& ViewportPosition, const bool bPlayerViewportRelative,
                                                                         const bool bCalculateOutsideScreenLocation)
{
	FVector2f NormalizedViewportLocation;
	if (!UFuCoordinateSpaceUtility::TryTransformWorldToNormalizedViewportLocationLocalPlayer(
		LocalPlayer, WorldLocation, NormalizedViewportLocation, bPlayerViewportRelative, bCalculateOutsideScreenLocation))
	{
		ViewportPosition = ImVec2{};
		return false;
	}

	const auto* Viewport{ImGui::GetMainViewport()};

	ViewportPosition = NormalizedViewportLocation * Viewport->Size + Viewport->Pos;
	return true;
}

bool UFuImGuiUtility::TryTransformWorldToMainViewportPosition(const APlayerController* Player, const FVector& WorldLocation,
                                                              ImVec2& ViewportPosition, const bool bPlayerViewportRelative,
                                                              const bool bCalculateOutsideScreenLocation)
{
	if (!IsValid(Player))
	{
		ViewportPosition = ImVec2{};
		return false;
	}

	return TryTransformWorldToMainViewportPositionLocalPlayer(Player->GetLocalPlayer(), WorldLocation, ViewportPosition,
	                                                          bPlayerViewportRelative, bCalculateOutsideScreenLocation);
}
#endif
