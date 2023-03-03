#include "Session/FuAsyncAction_JoinSession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_JoinSession)

UFuAsyncAction_JoinSession* UFuAsyncAction_JoinSession::FuJoinSession(APlayerController* Player,
                                                                      const FBlueprintSessionResult& SearchResult,
                                                                      const bool bTravelOnSuccess)
{
	auto* Task{NewObject<UFuAsyncAction_JoinSession>()};

	Task->Player1 = Player;
	Task->SearchResult1 = SearchResult;
	Task->bTravelOnSuccess1 = bTravelOnSuccess;

	return Task;
}

void UFuAsyncAction_JoinSession::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Player1.IsValid()) || !FU_ENSURE(IsValid(Player1->PlayerState)))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const auto Session{Online::GetSessionInterface(Player1->GetWorld())};
	if (!FU_ENSURE(Session.IsValid()))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnJoined));

	Session->JoinSession(*Player1->PlayerState->GetUniqueId().GetUniqueNetId(), NAME_GameSession, SearchResult1.OnlineResult);
}

void UFuAsyncAction_JoinSession::Session_OnJoined(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result)
{
	if (!Player1.IsValid())
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const auto Session{Online::GetSessionInterface(Player1->GetWorld())};
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

	if (bTravelOnSuccess1)
	{
		Player1->ClientTravel(ConnectString, TRAVEL_Absolute);
	}

	OnSuccess.Broadcast();
	SetReadyToDestroy();
}
