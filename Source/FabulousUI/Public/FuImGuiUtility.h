#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#ifdef WITH_IMGUI
THIRD_PARTY_INCLUDES_START
#include <imgui.h>
THIRD_PARTY_INCLUDES_END
#endif

#include "FuImGuiUtility.generated.h"

class ULocalPlayer;

/// In order for functions inside @b UFuImGuiUtility to be available:
/// 1. Add the <a href="https://github.com/VesCodes/ImGui">ImGui for Unreal Engine</a>  plugin to your project.
/// 2. Add the following lines to your project's @b .uproject file:
/// @code
/// "Plugins": [
///	    {
///	        "Name": "ImGui",
///	        "Enabled": true
///	    }
///	]
/// @endcode
UCLASS()
class FABULOUSUI_API UFuImGuiUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#if defined(WITH_IMGUI) && !defined(IMGUI_DISABLE)
public:
	static ImVec4 AdjustHueAndSaturation(const ImVec4& Color, float HueShift, float SaturationScale);

	static bool IsKeysPressed(const TArray<FKey>& Keys);

	static bool TryTransformWorldToMainViewportPositionLocalPlayer(const ULocalPlayer* LocalPlayer, const FVector& WorldLocation,
	                                                               ImVec2& ViewportPosition, bool bPlayerViewportRelative = false,
	                                                               bool bCalculateOutsideScreenLocation = true);

	static bool TryTransformWorldToMainViewportPosition(const APlayerController* Player, const FVector& WorldLocation,
	                                                    ImVec2& ViewportPosition, bool bPlayerViewportRelative = false,
	                                                    bool bCalculateOutsideScreenLocation = true);

	/// Draws a search box of the requested width, with a button clearing it displayed inside it, at its right
	/// end, and returns whether the text of the filter has changed, in which case that filter has already
	/// been rebuilt. A width of zero or less requests all the free horizontal space left in the current
	/// window or child. Use instead of ImGuiTextFilter::Draw(), which draws nothing but a plain input field.
	static bool SearchBox(ImGuiTextFilter& TextFilter, const char* Label = nullptr, const char* Hint = nullptr, float Width = 0.0f);
#endif
};
