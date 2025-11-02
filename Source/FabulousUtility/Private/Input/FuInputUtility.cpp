#include "Input/FuInputUtility.h"

#include "FuMacros.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Misc/ConfigCacheIni.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuInputUtility)

float UFuInputUtility::GetInitialButtonRepeatDelay()
{
	return GConfig->GetFloatOrDefault(TEXT("/Script/Engine.InputSettings"), TEXT("InitialButtonRepeatDelay"), 0.0f, GInputIni);
}

float UFuInputUtility::GetButtonRepeatDelay()
{
	return GConfig->GetFloatOrDefault(TEXT("/Script/Engine.InputSettings"), TEXT("ButtonRepeatDelay"), 0.0f, GInputIni);
}

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

void UFuInputUtility::SetInputModeGameOnly(APlayerController* Player, const bool bConsumeCaptureMouseDown)
{
	if (FU_ENSURE(IsValid(Player)))
	{
		FInputModeGameOnly GameOnlyInputMode;
		GameOnlyInputMode.SetConsumeCaptureMouseDown(bConsumeCaptureMouseDown);

		Player->SetInputMode(GameOnlyInputMode);
	}
}
