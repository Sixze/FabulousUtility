#pragma once

#include "FindSessionsCallbackProxy.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FuAsyncAction_JoinSession.generated.h"

UCLASS(DisplayName = "Fu Join Session Async Action")
class FABULOUSONLINE_API UFuAsyncAction_JoinSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<APlayerController> Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FBlueprintSessionResult SearchResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bTravelOnSuccess;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InPlayer"))
	static UFuAsyncAction_JoinSession* FuJoinSession(UPARAM(DisplayName = "Player") APlayerController* InPlayer,
	                                                 UPARAM(DisplayName = "Search Result") const FBlueprintSessionResult& InSearchResult,
	                                                 UPARAM(DisplayName = "Travel on Success") bool bInTravelOnSuccess = true);

	virtual void Activate() override;

private:
	void Session_OnJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
