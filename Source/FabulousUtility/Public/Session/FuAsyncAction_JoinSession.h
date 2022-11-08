#pragma once

#include "FindSessionsCallbackProxy.h"
#include "FuAsyncAction_JoinSession.generated.h"

UCLASS(DisplayName = "Fu Join Session Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_JoinSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Player")
	TWeakObjectPtr<APlayerController> Player1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Search Result")
	FBlueprintSessionResult SearchResult1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Travel on Success")
	bool bTravelOnSuccess1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Player"))
	static UFuAsyncAction_JoinSession* FuJoinSession(APlayerController* Player, const FBlueprintSessionResult& SearchResult,
	                                                 bool bTravelOnSuccess = true);

	virtual void Activate() override;

private:
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
