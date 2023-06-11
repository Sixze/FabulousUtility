#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_Delay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuDelayDelegate, int32, LoopIndex);

UCLASS(DisplayName = "Fu Delay Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_Delay : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (ClampMin = 0, ForceUnits = "s"))
	float Duration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	int32 LoopsCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bSkipFirstDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (ClampMin = 0))
	int32 LoopIndex{-1};

	FTimerHandle TimerHandle;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Delay Ability Task")
	FFuDelayDelegate OnLoop;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Delay Ability Task")
	FFuDelayDelegate OnDelayEnded;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_Delay* FuWaitForDelay(UGameplayAbility* OwningAbility,
	                                            UPARAM(DisplayName = "Duration") float InDuration = 0.2f,
	                                            UPARAM(DisplayName = "Loop Count") int32 InLoopsCount = 0,
	                                            UPARAM(DisplayName = "Skip First Delay") bool bInSkipFirstDelay = false);

protected:
	virtual void Activate() override;

public:
	virtual FString GetDebugString() const override;

protected:
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void TimerManager_OnTimerEnded();
};
