#include "FuNetworkingUtility.h"

#include "FuMacros.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuNetworkingUtility)

bool UFuNetworkingUtility::IsActorLocallyControlled(const AActor* Actor)
{
	if (!FU_ENSURE(IsValid(Actor)))
	{
		return false;
	}

	const auto* Player{Cast<APlayerController>(Actor)};
	if (IsValid(Player))
	{
		return Player->IsLocalController();
	}

	const auto* Pawn{Cast<APawn>(Actor)};
	if (IsValid(Pawn))
	{
		return Pawn->IsLocallyControlled();
	}

	Player = Cast<APlayerController>(Actor->GetNetOwner());
	if (IsValid(Player))
	{
		return Player->IsLocalController();
	}

	return false;
}
