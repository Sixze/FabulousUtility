#include "Session/FuAsyncAction_CreateSession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

UFuAsyncAction_CreateSession* UFuAsyncAction_CreateSession::FuCreateSession(APlayerController* Player, const int32 PublicConnections,
                                                                            const bool bLanOnly)
{
	auto* Task{NewObject<UFuAsyncAction_CreateSession>()};

	Task->Player1 = Player;
	Task->PublicConnections1 = FMath::Max(0, PublicConnections);
	Task->bLanOnly1 = bLanOnly;

	return Task;
}

void UFuAsyncAction_CreateSession::Activate()
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

	Session->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnCreated));

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = PublicConnections1;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = bLanOnly1;
	Settings.bUsesPresence = true;
	Settings.bAllowJoinViaPresence = true;
	Settings.bUseLobbiesIfAvailable = true;

	Session->CreateSession(*Player1->PlayerState->GetUniqueId().GetUniqueNetId(), NAME_GameSession, Settings);
}

void UFuAsyncAction_CreateSession::Session_OnCreated(const FName SessionName, const bool bSuccess)
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

	Session->ClearOnCreateSessionCompleteDelegates(this);

	if (!bSuccess)
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnStarted));

	if (!Session->StartSession(NAME_GameSession))
	{
		Session->ClearOnStartSessionCompleteDelegates(this);

		OnFailure.Broadcast();
		SetReadyToDestroy();
	}
}

void UFuAsyncAction_CreateSession::Session_OnStarted(const FName SessionName, const bool bSuccess)
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

	Session->ClearOnStartSessionCompleteDelegates(this);

	if (!bSuccess)
	{
		OnFailure.Broadcast();
		SetReadyToDestroy();
		return;
	}

	OnSuccess.Broadcast();
	SetReadyToDestroy();
}
