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
	if (!IsValid(Actor))
	{
		Controller = nullptr;
		return false;
	}

	Controller = Cast<AController>(Actor);
	if (IsValid(Controller))
	{
		return true;
	}

	const auto* Pawn{Cast<APawn>(Actor)};

	Controller = IsValid(Pawn) ? Pawn->GetController() : nullptr;
	return IsValid(Controller);
}

APlayerController* UFuControllerUtility::GetPlayerController(AActor* Actor)
{
	return Cast<APlayerController>(GetController(Actor));
}

bool UFuControllerUtility::TryGetPlayerController(AActor* Actor, APlayerController*& Player)
{
	AController* Controller;
	if (!TryGetController(Actor, Controller))
	{
		Player = nullptr;
		return false;
	}

	Player = Cast<APlayerController>(Controller);
	return IsValid(Player);
}
