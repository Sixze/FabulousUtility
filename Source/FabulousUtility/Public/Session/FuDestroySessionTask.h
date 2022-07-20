#pragma once

#include "Net/OnlineBlueprintCallProxyBase.h"
#include "FuDestroySessionTask.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuDestroySessionTask : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Player Controller", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<APlayerController> PlayerController1;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Destroy Session", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "PlayerController"))
	static UFuDestroySessionTask* FuDestroySession(APlayerController* PlayerController);

	virtual void Activate() override;

private:
	void OnDestroySessionComplete(FName SessionName, bool bSuccess) const;
};
