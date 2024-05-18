#include "AsyncActions/FuAsyncAction_CreateSession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_CreateSession)

UFuAsyncAction_CreateSession* UFuAsyncAction_CreateSession::CreateSession(APlayerController* InPlayer, const int32 InMaxPublicConnections,
                                                                          const bool bInLanOnly)
{
	auto* Task{NewObject<UFuAsyncAction_CreateSession>()};

	Task->Player = InPlayer;
	Task->MaxPublicConnections = FMath::Max(0, InMaxPublicConnections);
	Task->bLanOnly = bInLanOnly;

	return Task;
}

void UFuAsyncAction_CreateSession::Activate()
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

	Session->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::Session_OnCreated));

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = MaxPublicConnections;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = bLanOnly;
	Settings.bUsesPresence = true;
	Settings.bAllowJoinViaPresence = true;
	Settings.bUseLobbiesIfAvailable = true;

	Session->CreateSession(*Player->PlayerState->GetUniqueId().GetUniqueNetId(), NAME_GameSession, Settings);
}

void UFuAsyncAction_CreateSession::Session_OnCreated(const FName SessionName, const bool bSuccess)
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
