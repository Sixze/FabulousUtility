#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuOSUtility.generated.h"

UCLASS(DisplayName = "Fu OS Utility")
class FABULOUSUTILITY_API UFuOSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Fu OS Utility")
	static void FlashUnfocusedWindow();
};
