#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_OneFrameDelay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuOneFrameDelayEndedDelegate);

UCLASS(DisplayName = "Fu One Frame Delay Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_OneFrameDelay : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu One Frame Delay Ability Task")
	FFuOneFrameDelayEndedDelegate OnDelayEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_OneFrameDelay* FuWaitForOneFrameDelay(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void OnTimerEnded();
};
