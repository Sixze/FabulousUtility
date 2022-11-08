#pragma once

#include "FuAbilityTask_TargetDataSender.h"
#include "FuAbilityTask_TargetDataReceiver.generated.h"

// In most cases, you probably won't need to use UFuAbilityTask_TargetDataReceiver because
// UFuAbilityTask_TargetDataSender also handles receiving target data on the server.
UCLASS(DisplayName = "Fu Target Data Receiver Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_TargetDataReceiver : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Receive Once")
	bool bReceiveOnce1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Target Data Receiver Ability Task")
	FFuTargetDataSenderDelegate OnTargetDataReceived;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TargetDataReceiver* FuReceiveTargetData(UGameplayAbility* OwningAbility, bool bReceiveOnce = false);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag ActivationTag);
};
