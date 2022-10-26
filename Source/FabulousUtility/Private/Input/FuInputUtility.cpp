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

void UFuInputUtility::FuSetInputModeGameOnly(APlayerController* Player, const bool bConsumeCaptureMouseDown)
{
	if (FU_ENSURE(IsValid(Player)))
	{
		FInputModeGameOnly GameOnlyInputMode;
		GameOnlyInputMode.SetConsumeCaptureMouseDown(bConsumeCaptureMouseDown);

		Player->SetInputMode(GameOnlyInputMode);
	}
}
