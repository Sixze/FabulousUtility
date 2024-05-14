#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_OneFrameDelay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuOneFrameDelayDelegate);

UCLASS(DisplayName = "Fu One Frame Delay Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_OneFrameDelay : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|One Frame Delay")
	FFuOneFrameDelayDelegate OnDelayEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_OneFrameDelay* WaitForOneFrameDelay(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void TimerManager_OnTimerEnded();
};
