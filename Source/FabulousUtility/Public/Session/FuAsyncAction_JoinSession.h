#pragma once

#include "FindSessionsCallbackProxy.h"
#include "FuAsyncAction_JoinSession.generated.h"

UCLASS(DisplayName = "Fu Join Session Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_JoinSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Player", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<APlayerController> Player1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Search Result", Meta = (AllowPrivateAccess))
	FBlueprintSessionResult SearchResult1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Delay Before Travel", Meta = (AllowPrivateAccess, ForceUnits = "s"))
	float DelayBeforeTravel1;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnTravelDelayStarted;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Join Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Player"))
	static UFuAsyncAction_JoinSession* FuJoinSession(APlayerController* Player, const FBlueprintSessionResult& SearchResult,
	                                                 float DelayBeforeTravel = 0.0f);

	virtual void Activate() override;

private:
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnDelayBeforeTravelEnded();
};
