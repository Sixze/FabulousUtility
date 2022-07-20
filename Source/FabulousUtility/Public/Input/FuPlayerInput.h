#pragma once

#include "GameFramework/PlayerInput.h"
#include "FuPlayerInput.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuPlayerInput : public UPlayerInput
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Player Input")
	void RetriggerPressedActionsAndKeys(const TArray<UInputComponent*>& InputStack, bool bBlockedInputComponentsOnly) const;

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Player Input")
	void ReleasePressedActionsAndKeys(const TArray<UInputComponent*>& InputStack, bool bBlockedInputComponentsOnly) const;

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Player Input")
	void TriggerInputEventForPressedActionsAndKeys(const TArray<UInputComponent*>& InputStack,
	                                               EInputEvent InputEvent, bool bBlockedInputComponentsOnly) const;
};
