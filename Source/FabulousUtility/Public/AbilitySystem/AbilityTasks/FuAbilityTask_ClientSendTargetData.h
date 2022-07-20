#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_ClientSendTargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuTargetDataReceivedDelegate, const FGameplayAbilityTargetDataHandle&, TargetData);

UCLASS(DisplayName = "Fu Client Send Target Data Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_ClientSendTargetData : public UAbilityTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Target Data", Meta = (AllowPrivateAccess))
	FGameplayAbilityTargetDataHandle TargetData1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Client Send Target Data Ability Task")
	FFuTargetDataReceivedDelegate OnTargetDataReceived;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_ClientSendTargetData* FuClientSendTargetData(UGameplayAbility* OwningAbility,
	                                                                   const FGameplayAbilityTargetDataHandle& TargetData);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag ActivationTag);
};
