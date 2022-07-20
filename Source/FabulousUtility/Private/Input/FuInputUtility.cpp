#include "Input/FuInputUtility.h"

#include "FuMacros.h"
#include "GameFramework/PlayerController.h"

void UFuInputUtility::SetBlockInput(AActor* Actor, const bool bBlockInput)
{
	if (FU_ENSURE(IsValid(Actor)))
	{
		Actor->bBlockInput = bBlockInput;

		if (IsValid(Actor->InputComponent))
		{
			Actor->InputComponent->bBlockInput = bBlockInput;
		}
	}
}

void UFuInputUtility::FuSetInputModeGameOnly(APlayerController* PlayerController, const bool bConsumeCaptureMouseDown)
{
	if (FU_ENSURE(IsValid(PlayerController)))
	{
		FInputModeGameOnly GameOnlyInputMode;
		GameOnlyInputMode.SetConsumeCaptureMouseDown(bConsumeCaptureMouseDown);

		PlayerController->SetInputMode(GameOnlyInputMode);
	}
}
