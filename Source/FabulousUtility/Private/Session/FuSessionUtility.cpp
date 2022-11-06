#include "Session/FuSessionUtility.h"

#include "FuMacros.h"
#include "GameFramework/PlayerController.h"
#include "Net/OnlineEngineInterface.h"

bool UFuSessionUtility::TryTravelToGameSession(APlayerController* Player)
{
	if (!FU_ENSURE(IsValid(Player)))
	{
		return false;
	}

	FString ConnectString;
	if (!FU_ENSURE_ALWAYS(UOnlineEngineInterface::Get()->GetResolvedConnectString(Player->GetWorld(), NAME_GameSession, ConnectString)))
	{
		return false;
	}

	Player->ClientTravel(ConnectString, TRAVEL_Absolute);
	return true;
}
