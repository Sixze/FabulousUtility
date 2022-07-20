#pragma once

#include "FindSessionsCallbackProxy.h"
#include "Engine/EngineTypes.h"
#include "FuCreateSessionTask.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuCreateSessionTask : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Player Controller", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<APlayerController> PlayerController1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Public Connections", Meta = (AllowPrivateAccess))
	int32 PublicConnections1{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Use Lan", Meta = (AllowPrivateAccess))
	bool bUseLan1;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Create Session", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Create Session", Meta = (AllowPrivateAccess))
	FEmptyOnlineDelegate OnFailure;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session",
		BlueprintInternalUseOnly, Meta = (DefaultToSelf = "PlayerController"))
	static UFuCreateSessionTask* FuCreateSession(APlayerController* PlayerController, int32 PublicConnections, bool bUseLan);

	virtual void Activate() override;

private:
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);

	void OnStartSessionComplete(FName SessionName, bool bSuccess);
};
