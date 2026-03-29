#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSlateUtility.generated.h"

struct FSlateSound;
class SWidget;

UENUM(BlueprintType)
enum class EFuFocusableWidgetsSearchMode : uint8
{
	/// Return the topmost focusable widgets, even if there are other focusable widgets in their hierarchy.
	AncestorWidgets,

	/// Return the deepest focusable widgets. However, if one of those widgets has a focusable ancestor and is the only
	/// focusable widget in its ancestor hierarchy, the ancestor itself will be returned instead of the descendant.
	AncestorWidgetsWithSingleDescendant,

	/// Return the deepest focusable widgets.
	DescendantWidgets,
};

UCLASS()
class FABULOUSUI_API UFuSlateUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous UI|Slate Utility")
	static void PlaySound(const FSlateSound& Sound);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Fabulous UI|Slate Utility", Meta = (ReturnDisplayName = "Dpi Scale"))
	static float GetViewportDpiScale(FVector2D ViewportSize);

	static void FindFocusableWidgets(SWidget& RootWidget, EFuFocusableWidgetsSearchMode SearchMode,
	                                 TArray<TSharedRef<SWidget>>& FoundWidgets);
};
