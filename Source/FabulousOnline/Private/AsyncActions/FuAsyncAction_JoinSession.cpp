#include "AsyncActions/FuAsyncAction_JoinSession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_JoinSession)

UFuAsyncAction_JoinSession* UFuAsyncAction_JoinSession::FuJoinSession(APlayerController* InPlayer,
                                                                      const FBlueprintSessionResult& InSearchResult,
                                                                      const bool bInTravelOnSuccess)
{
	auto* Task{NewObject<UFuAsyncAction_JoinSession>()};

	Task->Player = InPlayer;
	Task->SearchResult = InSearchResult;
	Task->bTravelOnSuccess = bInTravelOnSuccess;

	return Task;
}

void UFuAsyncAction_JoinSession::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Player.IsValid()) || !FU_ENSURE(IsValid(Player->PlayerState)))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const auto Session{Online::GetSessionInterface(Player->GetWorld())};
	if (!FU_ENSURE(Session.IsValid()))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnJoined));

	Session->JoinSession(*Player->PlayerState->GetUniqueId().GetUniqueNetId(), NAME_GameSession, SearchResult.OnlineResult);
}

void UFuAsyncAction_JoinSession::Session_OnJoined(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result)
{
	if (!Player.IsValid())
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const auto Session{Online::GetSessionInterface(Player->GetWorld())};
	if (!Session.IsValid())
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Session->ClearOnJoinSessionCompleteDelegates(this);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	FString ConnectString;
	if (!Session->GetResolvedConnectString(NAME_GameSession, ConnectString))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	if (bTravelOnSuccess)
	{
		Player->ClientTravel(ConnectString, TRAVEL_Absolute);
	}

	OnSuccess.Broadcast();
	SetReadyToDestroy();
}
