#include "Session/FuAsyncAction_JoinSession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

UFuAsyncAction_JoinSession* UFuAsyncAction_JoinSession::FuJoinSession(
	APlayerController* Player, const FBlueprintSessionResult& SearchResult, const float DelayBeforeTravel)
{
	auto* Task{NewObject<UFuAsyncAction_JoinSession>()};

	Task->Player1 = Player;
	Task->SearchResult1 = SearchResult;
	Task->DelayBeforeTravel1 = DelayBeforeTravel;

	return Task;
}

void UFuAsyncAction_JoinSession::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Player1.IsValid()))
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

	Session->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete));

	if (!FU_ENSURE(IsValid(Player1->PlayerState)) ||
	    !Session->JoinSession(*Player1->PlayerState->GetUniqueId().GetUniqueNetId(),
	                          NAME_GameSession, SearchResult1.OnlineResult))
	{
		Session->ClearOnJoinSessionCompleteDelegates(this);

		OnFailure.Broadcast();
		SetReadyToDestroy();
	}
}

void UFuAsyncAction_JoinSession::OnJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result)
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

	OnTravelDelayStarted.Broadcast();

	if (DelayBeforeTravel1 <= SMALL_NUMBER)
	{
		OnDelayBeforeTravelEnded();
		return;
	}

	FTimerHandle TimerHandle;
	Player1->GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnDelayBeforeTravelEnded, DelayBeforeTravel1, false);
}

void UFuAsyncAction_JoinSession::OnDelayBeforeTravelEnded()
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

	FString ConnectString;
	if (!Session->GetResolvedConnectString(NAME_GameSession, ConnectString))
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Player1->ClientTravel(ConnectString, TRAVEL_Absolute);

	OnSuccess.Broadcast();
	SetReadyToDestroy();
}
