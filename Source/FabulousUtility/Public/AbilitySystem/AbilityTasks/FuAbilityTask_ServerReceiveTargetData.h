#pragma once

#include "FuAbilityTask_ClientSendTargetData.h"
#include "FuAbilityTask_ServerReceiveTargetData.generated.h"

UCLASS(DisplayName = "Fu Server Receive Target Data Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_ServerReceiveTargetData : public UAbilityTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Receive Once", Meta = (AllowPrivateAccess))
	bool bReceiveOnce1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Server Receive Target Data Ability Task")
	FFuTargetDataReceivedDelegate OnTargetDataReceived;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_ServerReceiveTargetData* FuServerWaitForTargetData(UGameplayAbility* OwningAbility, bool bReceiveOnce = false);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag ActivationTag);
};
