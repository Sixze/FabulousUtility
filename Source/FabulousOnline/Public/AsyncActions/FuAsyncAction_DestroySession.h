#pragma once

#include "Net/OnlineBlueprintCallProxyBase.h"
#include "FuAsyncAction_DestroySession.generated.h"

UCLASS(DisplayName = "Fu Destroy Session Async Action")
class FABULOUSONLINE_API UFuAsyncAction_DestroySession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<APlayerController> Player;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session Async Action")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session Async Action")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InPlayer"))
	static UFuAsyncAction_DestroySession* FuDestroySession(UPARAM(DisplayName = "Player") APlayerController* InPlayer);

	virtual void Activate() override;

private:
	void Session_OnDestroyed(FName SessionName, bool bSuccess);
};
