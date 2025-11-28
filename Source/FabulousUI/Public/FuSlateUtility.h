#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSlateUtility.generated.h"

class SWidget;

UENUM(BlueprintType)
enum class EFuFocusableWidgetsSearchMode : uint8
{
	// Returns topmost focusable widgets, even if they also have focusable widgets in their hierarchy.
	AncestorWidgets,

	// Returns deepest focusable widgets, but if one of those widgets has a focusable ancestor, and it is the only
	// focusable widget in the ancestor hierarchy, then the ancestor itself will be returned instead of the descendant.
	AncestorWidgetsWithSingleDescendant,

	// Returns deepest focusable widgets.
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
