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

bool UFuImGuiUtility::SearchBox(ImGuiTextFilter& TextFilter, const char* Label, const char* Hint, const float Width)
{
	const auto BoxPosition{ImGui::GetCursorScreenPos()};
	const auto BoxWidth{Width <= 0.0f ? ImGui::GetContentRegionAvail().x : Width};
	const auto BoxHeight{ImGui::GetFrameHeight()};

	// The clear button takes the square right end of the box, and is only drawn while that box has
	// something to clear, so an empty box is not cluttered with a button that would do nothing at all.

	const auto bShowClearButton{TextFilter.InputBuf[0] != '\0'};
	const ImVec2 ClearButtonPosition{BoxPosition.x + BoxWidth - BoxHeight, BoxPosition.y};

	auto bClearButtonPressed{false};
	auto bClearButtonHovered{false};
	auto bClearButtonHeld{false};

	ImGui::PushID(Label);

	if (bShowClearButton)
	{
		// The button is submitted before the box it overlaps, because Dear ImGui refuses
		// to hover an item  overlapping the active one while a mouse button is held down.

		ImGui::SetCursorScreenPos(ClearButtonPosition);

		bClearButtonPressed = ImGui::InvisibleButton("##ClearButton", ImVec2{BoxHeight, BoxHeight});
		bClearButtonHovered = ImGui::IsItemHovered();
		bClearButtonHeld = ImGui::IsItemActive();

		ImGui::SetCursorScreenPos(BoxPosition);
	}

	auto bTextChanged{false};

	if (bClearButtonPressed)
	{
		TextFilter.Clear();
		bTextChanged = true;
	}

	ImGui::SetNextItemWidth(BoxWidth);

	if (ImGui::InputTextWithHint(Label, Hint, TextFilter.InputBuf, UE_ARRAY_COUNT(TextFilter.InputBuf)))
	{
		TextFilter.Build();
		bTextChanged = true;
	}

	if (bShowClearButton)
	{
		auto* DrawList{ImGui::GetWindowDrawList()};

		if (bClearButtonHovered || bClearButtonHeld)
		{
			// The highlight is inset by the border of the box, and rounded exactly like its right end, so the
			// button looks like a part of that box instead of a button drawn on top of it.

			const auto& Style{ImGui::GetStyle()};

			const auto ButtonColor{
				ImGui::GetColorU32(bClearButtonHeld ? ImGuiCol_ButtonActive : ImGuiCol_ButtonHovered)
			};

			DrawList->AddRectFilled(ImVec2{
				                        ClearButtonPosition.x,
				                        ClearButtonPosition.y + Style.FrameBorderSize
			                        },
			                        ImVec2{
				                        ClearButtonPosition.x + BoxHeight - Style.FrameBorderSize,
				                        ClearButtonPosition.y + BoxHeight - Style.FrameBorderSize
			                        },
			                        ButtonColor, Style.FrameRounding, ImDrawFlags_RoundCornersRight);
		}

		const ImVec2 CrossCenter{ClearButtonPosition.x + 0.5f * BoxHeight, ClearButtonPosition.y + 0.5f * BoxHeight};
		const auto CrossExtent{ImGui::GetFontSize() * 0.5f * 0.7071f - 1.0f};

		const auto CrossColor{ImGui::GetColorU32(bClearButtonHovered || bClearButtonHeld ? ImGuiCol_Text : ImGuiCol_TextDisabled)};
		static constexpr auto CrossThickness{1.0f};

		DrawList->AddLine(ImVec2{CrossCenter.x + CrossExtent, CrossCenter.y + CrossExtent},
		                  ImVec2{CrossCenter.x - CrossExtent, CrossCenter.y - CrossExtent},
		                  CrossColor, CrossThickness);

		DrawList->AddLine(ImVec2{CrossCenter.x + CrossExtent, CrossCenter.y - CrossExtent},
		                  ImVec2{CrossCenter.x - CrossExtent, CrossCenter.y + CrossExtent},
		                  CrossColor, CrossThickness);
	}

	ImGui::PopID();

	return bTextChanged;
}
#endif
