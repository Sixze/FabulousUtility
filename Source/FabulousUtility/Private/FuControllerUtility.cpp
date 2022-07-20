#include "FuControllerUtility.h"

#include "FuMacros.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

bool UFuControllerUtility::HasController(AActor* Actor)
{
	return IsValid(GetController(Actor));
}

AController* UFuControllerUtility::GetController(AActor* Actor)
{
	if (!FU_ENSURE(IsValid(Actor)))
	{
		return nullptr;
	}

	auto* Controller{Cast<AController>(Actor)};
	if (IsValid(Controller))
	{
		return Controller;
	}

	const auto* Pawn{Cast<APawn>(Actor)};
	if (IsValid(Pawn))
	{
		return Pawn->GetController();
	}

	return nullptr;
}

bool UFuControllerUtility::HasPlayerController(AActor* Actor)
{
	return IsValid(GetPlayerController(Actor));
}

APlayerController* UFuControllerUtility::GetPlayerController(AActor* Actor)
{
	return Cast<APlayerController>(GetController(Actor));
}
