#pragma once

#include "FindSessionsCallbackProxy.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FuAsyncAction_JoinSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuJoinSessionDelegate, const FString&, FailureString);

UCLASS(DisplayName = "Fu Join Session Async Action")
class FABULOUSONLINE_API UFuAsyncAction_JoinSession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	static inline const FString UnknownFailureString{TEXTVIEW("Failed to Join Session")};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<APlayerController> Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FBlueprintSessionResult SearchResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bTravelOnSuccess : 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Join Session Async Action")
	FFuJoinSessionDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Join Session Async Action")
	FFuJoinSessionDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InPlayer"))
	static UFuAsyncAction_JoinSession* JoinSession(UPARAM(DisplayName = "Player") APlayerController* InPlayer,
	                                               UPARAM(DisplayName = "Search Result") const FBlueprintSessionResult& InSearchResult,
	                                               UPARAM(DisplayName = "Travel on Success") bool bInTravelOnSuccess = true);

	virtual void Activate() override;

private:
	void Session_OnJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
