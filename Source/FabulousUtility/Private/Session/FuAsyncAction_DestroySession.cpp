#include "Session/FuAsyncAction_DestroySession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"

UFuAsyncAction_DestroySession* UFuAsyncAction_DestroySession::FuDestroySession(APlayerController* Player)
{
	auto* Task{NewObject<UFuAsyncAction_DestroySession>()};

	Task->Player1 = Player;

	return Task;
}

void UFuAsyncAction_DestroySession::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Player1.IsValid()))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const auto Session{Online::GetSessionInterface(Player1->GetWorld())};

	if (!FU_ENSURE(Session.IsValid()) || Session->GetNamedSession(NAME_GameSession) == nullptr)
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Session->DestroySession(NAME_GameSession,
	                        FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnDestroySessionCompleted));
}

void UFuAsyncAction_DestroySession::Session_OnDestroySessionCompleted(const FName SessionName, const bool bSuccess)
{
	if (bSuccess)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}

	SetReadyToDestroy();
}
