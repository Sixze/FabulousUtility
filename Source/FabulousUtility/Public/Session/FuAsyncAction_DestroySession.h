#pragma once

#include "Net/OnlineBlueprintCallProxyBase.h"
#include "FuAsyncAction_DestroySession.generated.h"

UCLASS(DisplayName = "Fu Destroy Session Async Action")
class FABULOUSUTILITY_API UFuAsyncAction_DestroySession : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Player Controller", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<APlayerController> PlayerController1;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session Async Action", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "PlayerController"))
	static UFuAsyncAction_DestroySession* FuDestroySession(APlayerController* PlayerController);

	virtual void Activate() override;

private:
	void OnDestroySessionComplete(FName SessionName, bool bSuccess) const;
};
