#pragma once

#include "FindSessionsCallbackProxy.h"
#include "FuAsyncAction_CreateSession.generated.h"

UCLASS(DisplayName = "Fu Create Session Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_CreateSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Player")
	TWeakObjectPtr<APlayerController> Player1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Public Connections")
	int32 PublicConnections1{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Use Lan")
	bool bLanOnly1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Create Session Async Action")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Create Session Async Action")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Player"))
	static UFuAsyncAction_CreateSession* FuCreateSession(APlayerController* Player, int32 PublicConnections, bool bLanOnly);

	virtual void Activate() override;

private:
	void Session_OnCreated(FName SessionName, bool bSuccess);

	void Session_OnStarted(FName SessionName, bool bSuccess);
};
