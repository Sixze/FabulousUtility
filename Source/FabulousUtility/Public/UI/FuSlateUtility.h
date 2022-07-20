#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Sound/SlateSound.h"
#include "FuSlateUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuSlateUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Fu Slate Utility")
	static void PlaySound(const FSlateSound& Sound);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Fu Slate Utility",
		Meta = (AutoCreateRefTerm = "WidgetSize"))
	static float GetDpiScale(const FVector2D& WidgetSize);
};
