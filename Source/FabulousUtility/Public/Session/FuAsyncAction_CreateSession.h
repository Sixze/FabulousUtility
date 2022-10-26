#pragma once

#include "FindSessionsCallbackProxy.h"
#include "Engine/EngineTypes.h"
#include "FuAsyncAction_CreateSession.generated.h"

UCLASS(DisplayName = "Fu Create Session Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_CreateSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Player", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<APlayerController> Player1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Public Connections", Meta = (AllowPrivateAccess))
	int32 PublicConnections1{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Use Lan", Meta = (AllowPrivateAccess))
	bool bUseLan1;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Create Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Create Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Player"))
	static UFuAsyncAction_CreateSession* FuCreateSession(APlayerController* Player, int32 PublicConnections, bool bUseLan);

	virtual void Activate() override;

private:
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);

	void OnStartSessionComplete(FName SessionName, bool bSuccess);
};
