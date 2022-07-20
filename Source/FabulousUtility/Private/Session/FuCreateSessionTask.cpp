#include "Session/FuCreateSessionTask.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

UFuCreateSessionTask* UFuCreateSessionTask::FuCreateSession(APlayerController* PlayerController,
                                                            const int32 PublicConnections, const bool bUseLan)
{
	auto* Task{NewObject<UFuCreateSessionTask>()};

	Task->PlayerController1 = PlayerController;
	Task->PublicConnections1 = FMath::Max(0, PublicConnections);
	Task->bUseLan1 = bUseLan;

	return Task;
}

void UFuCreateSessionTask::Activate()
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

	Session->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete));

	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = PublicConnections1;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bIsLANMatch = bUseLan1;
	Settings.bUsesPresence = true;
	Settings.bAllowJoinViaPresence = true;
	Settings.bUseLobbiesIfAvailable = true;

	if (!FU_ENSURE(IsValid(PlayerController1->PlayerState)) ||
	    !Session->CreateSession(*PlayerController1->PlayerState->GetUniqueId().GetUniqueNetId(), NAME_GameSession, Settings))
	{
		Session->ClearOnCreateSessionCompleteDelegates(this);
		OnFailure.Broadcast();
	}
}

void UFuCreateSessionTask::OnCreateSessionComplete(const FName SessionName, const bool bSuccess)
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

	Session->ClearOnCreateSessionCompleteDelegates(this);

	if (!bSuccess)
	{
		OnFailure.Broadcast();
		return;
	}

	Session->AddOnStartSessionCompleteDelegate_Handle(
		FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete));

	if (!Session->StartSession(NAME_GameSession))
	{
		Session->ClearOnStartSessionCompleteDelegates(this);
		OnFailure.Broadcast();
	}
}

void UFuCreateSessionTask::OnStartSessionComplete(FName SessionName, bool bSuccess)
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

	Session->ClearOnStartSessionCompleteDelegates(this);

	if (!bSuccess)
	{
		OnFailure.Broadcast();
		return;
	}

	OnSuccess.Broadcast();
}
