#include "Session/FuAsyncAction_DestroySession.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"

UFuAsyncAction_DestroySession* UFuAsyncAction_DestroySession::FuDestroySession(APlayerController* PlayerController)
{
	auto* Task{NewObject<UFuAsyncAction_DestroySession>()};

	Task->PlayerController1 = PlayerController;

	return Task;
}

void UFuAsyncAction_DestroySession::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(PlayerController1.IsValid()))
	{
		OnFailure.Broadcast();
		return;
	}

	const auto Session{Online::GetSessionInterface(PlayerController1->GetWorld())};

	if (!FU_ENSURE(Session.IsValid()) || Session->GetNamedSession(NAME_GameSession) == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	Session->DestroySession(NAME_GameSession, FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete));
}

void UFuAsyncAction_DestroySession::OnDestroySessionComplete(const FName SessionName, const bool bSuccess) const
{
	if (bSuccess)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
