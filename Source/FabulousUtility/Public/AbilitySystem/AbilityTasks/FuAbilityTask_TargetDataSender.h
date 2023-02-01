#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_TargetDataSender.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuTargetDataSenderDelegate, const FGameplayAbilityTargetDataHandle&, TargetData);

UCLASS(DisplayName = "Fu Target Data Sender Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_TargetDataSender : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Target Data")
	FGameplayAbilityTargetDataHandle TargetData1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Target Data Sender Ability Task")
	FFuTargetDataSenderDelegate OnTargetDataReceived;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_TargetDataSender* FuSendTargetData(UGameplayAbility* OwningAbility,
	                                                         const FGameplayAbilityTargetDataHandle& TargetData);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void AbilitySystem_OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag ActivationTag);
};
