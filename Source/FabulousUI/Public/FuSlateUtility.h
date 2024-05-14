#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSlateUtility.generated.h"

UCLASS()
class FABULOUSUI_API UFuSlateUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous UI|Slate Utility")
	static void PlaySound(const FSlateSound& Sound);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Fabulous UI|Slate Utility", Meta = (ReturnDisplayName = "Dpi Scale"))
	static float GetViewportDpiScale(FVector2D ViewportSize);
};
