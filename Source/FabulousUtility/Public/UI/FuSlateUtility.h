#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSlateUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuSlateUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Fu Slate Utility")
	static void PlaySound(const FSlateSound& Sound);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Fu Slate Utility",
		Meta = (AutoCreateRefTerm = "ViewportSize", ReturnDisplayName = "Dpi Scale"))
	static float GetViewportDpiScale(FVector2D ViewportSize);
};
