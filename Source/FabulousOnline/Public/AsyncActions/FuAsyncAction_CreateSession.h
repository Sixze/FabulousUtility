#pragma once

#include "FindSessionsCallbackProxy.h"
#include "FuAsyncAction_CreateSession.generated.h"

UCLASS(DisplayName = "Fu Create Session Async Action")
class FABULOUSONLINE_API UFuAsyncAction_CreateSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<APlayerController> Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	int32 MaxPublicConnections;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bLanOnly : 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Online|Async Actions|Create Session")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Online|Async Actions|Create Session")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Online|Async Actions", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InPlayer"))
	static UFuAsyncAction_CreateSession* CreateSession(UPARAM(DisplayName = "Player") APlayerController* InPlayer,
	                                                   UPARAM(DisplayName = "Max Public Connections") int32 InMaxPublicConnections,
	                                                   UPARAM(DisplayName = "Lan Only") bool bInLanOnly);

	virtual void Activate() override;

private:
	void Session_OnCreated(FName SessionName, bool bSuccess);

	void Session_OnStarted(FName SessionName, bool bSuccess);
};
