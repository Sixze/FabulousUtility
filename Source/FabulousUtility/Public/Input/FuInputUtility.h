#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuInputUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuInputUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Input Utility", Meta = (DefaultToSelf = "Actor"))
	static void SetBlockInput(AActor* Actor, bool bBlockInput);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Input Utility", Meta = (DefaultToSelf = "Player"))
	static void FuSetInputModeGameOnly(APlayerController* Player, bool bConsumeCaptureMouseDown);
};
