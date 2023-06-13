#pragma once

#include "FuAbilityTask_TargetDataSender.h"
#include "FuAbilityTask_TargetDataReceiver.generated.h"

UCLASS(DisplayName = "Fu Target Data Receiver Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_TargetDataReceiver : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bReceiveOnce;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Target Data Receiver Ability Task")
	FFuTargetDataSenderDelegate OnTargetDataReceived;

public:
	// In most cases, you probably don't need to use UFuAbilityTask_TargetDataReceiver because
	// UFuAbilityTask_TargetDataSender also handles receiving target data on the server. Also,
	// this task works only on the server and does nothing in the standalone game or on clients.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TargetDataReceiver* FuReceiveTargetData(UGameplayAbility* OwningAbility,
	                                                              UPARAM(DisplayName = "Receive Once") bool bInReceiveOnce);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void AbilitySystem_OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag ActivationTag);
};
