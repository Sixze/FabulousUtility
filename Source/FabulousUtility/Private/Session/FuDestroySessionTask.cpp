#include "Session/FuDestroySessionTask.h"

#include "FuMacros.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerController.h"

UFuDestroySessionTask* UFuDestroySessionTask::FuDestroySession(APlayerController* PlayerController)
{
	auto* Task{NewObject<UFuDestroySessionTask>()};

	Task->PlayerController1 = PlayerController;

	return Task;
}

void UFuDestroySessionTask::Activate()
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

void UFuDestroySessionTask::OnDestroySessionComplete(const FName SessionName, const bool bSuccess) const
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
