#include "Session/FuJoinSessionTask.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

UFuJoinSessionTask* UFuJoinSessionTask::FuJoinSession(APlayerController* PlayerController,
                                                      const FBlueprintSessionResult& SearchResult,
                                                      const float DelayBeforeTravel)
{
	auto* Task{NewObject<UFuJoinSessionTask>()};

	Task->PlayerController1 = PlayerController;
	Task->SearchResult1 = SearchResult;
	Task->DelayBeforeTravel1 = DelayBeforeTravel;

	return Task;
}

void UFuJoinSessionTask::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(PlayerController1.IsValid()))
	{
		OnFailure.Broadcast();
		return;
	}

	const auto Session{Online::GetSessionInterface(PlayerController1->GetWorld())};
	if (!FU_ENSURE(Session.IsValid()))
	{
		OnFailure.Broadcast();
		return;
	}

	Session->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete));

	if (!FU_ENSURE(IsValid(PlayerController1->PlayerState)) ||
	    !Session->JoinSession(*PlayerController1->PlayerState->GetUniqueId().GetUniqueNetId(),
	                          NAME_GameSession, SearchResult1.OnlineResult))
	{
		Session->ClearOnJoinSessionCompleteDelegates(this);
		OnFailure.Broadcast();
	}
}

void UFuJoinSessionTask::OnJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result)
{
	if (!PlayerController1.IsValid())
	{
		OnFailure.Broadcast();
		return;
	}

	const auto Session{Online::GetSessionInterface(PlayerController1->GetWorld())};
	if (!Session.IsValid())
	{
		OnFailure.Broadcast();
		return;
	}

	Session->ClearOnJoinSessionCompleteDelegates(this);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		OnFailure.Broadcast();
		return;
	}

	OnTravelDelayStarted.Broadcast();

	if (DelayBeforeTravel1 <= SMALL_NUMBER)
	{
		OnDelayBeforeTravelEnded();
		return;
	}

	FTimerHandle TimerHandle;
	PlayerController1->GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnDelayBeforeTravelEnded,
	                                                   DelayBeforeTravel1, false);
}

void UFuJoinSessionTask::OnDelayBeforeTravelEnded() const
{
	if (!PlayerController1.IsValid())
	{
		OnFailure.Broadcast();
		return;
	}

	const auto Session{Online::GetSessionInterface(PlayerController1->GetWorld())};
	if (!Session.IsValid())
	{
		OnFailure.Broadcast();
		return;
	}

	FString ConnectString;
	if (!Session->GetResolvedConnectString(NAME_GameSession, ConnectString))
	{
		OnFailure.Broadcast();
		return;
	}

	PlayerController1->ClientTravel(ConnectString, TRAVEL_Absolute);
	OnSuccess.Broadcast();
}
