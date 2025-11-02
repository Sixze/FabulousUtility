#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuInputUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuInputUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Delay before sending a repeat message after a button was first pressed
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Input Utility", Meta = (ReturnDisplayName = "Initial Repeat Delay"))
	static float GetInitialButtonRepeatDelay();

	// Delay before sending a repeat message after a button has been pressed for a while.
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Input Utility", Meta = (ReturnDisplayName = "Repeat Delay"))
	static float GetButtonRepeatDelay();

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Input Utility", Meta = (DefaultToSelf = "Actor"))
	static void SetBlockInput(AActor* Actor, bool bBlockInput);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Input Utility", Meta = (DefaultToSelf = "Player"))
	static void SetInputModeGameOnly(APlayerController* Player, bool bConsumeCaptureMouseDown);
};
