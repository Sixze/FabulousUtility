#pragma once

#include "Net/OnlineBlueprintCallProxyBase.h"
#include "FuAsyncAction_DestroySession.generated.h"

UCLASS(DisplayName = "Fu Destroy Session Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_DestroySession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Player", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<APlayerController> Player1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session Async Action")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session Async Action")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "Player"))
	static UFuAsyncAction_DestroySession* FuDestroySession(APlayerController* Player);

	virtual void Activate() override;

private:
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
};
