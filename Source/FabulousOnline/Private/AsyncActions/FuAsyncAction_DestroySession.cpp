#include "AsyncActions/FuAsyncAction_DestroySession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_DestroySession)

UFuAsyncAction_DestroySession* UFuAsyncAction_DestroySession::FuDestroySession(APlayerController* InPlayer)
{
	auto* Task{NewObject<UFuAsyncAction_DestroySession>()};

	Task->Player = InPlayer;

	return Task;
}

void UFuAsyncAction_DestroySession::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Player.IsValid()))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const auto Session{Online::GetSessionInterface(Player->GetWorld())};

	if (!FU_ENSURE(Session.IsValid()) || Session->GetNamedSession(NAME_GameSession) == nullptr)
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Session->DestroySession(NAME_GameSession, FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnDestroyed));
}

void UFuAsyncAction_DestroySession::Session_OnDestroyed(const FName SessionName, const bool bSuccess)
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
