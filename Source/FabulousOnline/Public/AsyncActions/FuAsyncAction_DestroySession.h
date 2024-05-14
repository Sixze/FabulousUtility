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
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Online|Async Actions|Destroy Session")
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Online|Async Actions|Destroy Session")
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Online|Async Actions", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InPlayer"))
	static UFuAsyncAction_DestroySession* DestroySession(UPARAM(DisplayName = "Player") APlayerController* InPlayer);

	virtual void Activate() override;

private:
	void Session_OnDestroyed(FName SessionName, bool bSuccess);
};
