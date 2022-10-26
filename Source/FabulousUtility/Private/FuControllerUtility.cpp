#include "FuControllerUtility.h"

#include "FuMacros.h"
#include "GameFramework/Pawn.h"

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

bool UFuControllerUtility::TryGetController(AActor* Actor, AController*& Controller)
{
	Controller = GetController(Actor);
	return IsValid(Controller);
}

APlayerController* UFuControllerUtility::GetPlayerController(AActor* Actor)
{
	return Cast<APlayerController>(GetController(Actor));
}

bool UFuControllerUtility::TryGetPlayerController(AActor* Actor, APlayerController*& Player)
{
	Player = GetPlayerController(Actor);
	return IsValid(Player);
}
