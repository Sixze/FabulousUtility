#include "AbilitySystem/AbilityTasks/FuAbilityTask_Delay.h"

#include "AbilitySystemGlobals.h"
#include "TimerManager.h"

UFuAbilityTask_Delay* UFuAbilityTask_Delay::FuWaitForDelay(UGameplayAbility* OwningAbility, float Duration,
                                                           const int32 LoopsCount, const bool bSkipFirstDelay)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Duration);

	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->Duration1 = FMath::Max(0.0f, Duration);
	Task->LoopsCount1 = FMath::Max(-1, LoopsCount);
	Task->bSkipFirstDelay1 = bSkipFirstDelay;

	return Task;
}

void UFuAbilityTask_Delay::Activate()
{
	Super::Activate();

	if (Duration1 <= SMALL_NUMBER)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			for (auto i{0}; i < LoopsCount1; i++)
			{
				OnLoop.Broadcast(LoopIndex);
				LoopIndex += 1;
			}

			OnDelayEnded.Broadcast(LoopIndex);
		}

		EndTask();
		return;
	}

	if (bSkipFirstDelay1)
	{
		OnTimerEnded();
	}

	if (!IsFinished())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnTimerEnded, Duration1, LoopsCount1 != 0);
	}
}

FString UFuAbilityTask_Delay::GetDebugString() const
{
	return FString::Printf(TEXT("%s (%s): Time Remaining: %.2f, Loop Index: %d."), *GetName(), *InstanceName.ToString(),
	                       Duration1 - GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle), LoopIndex);
}

void UFuAbilityTask_Delay::OnDestroy(const bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_Delay::OnTimerEnded()
{
	if (IsFinished())
	{
		return;
	}

	if (LoopsCount1 < 0 || LoopIndex < LoopsCount1)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnLoop.Broadcast(LoopIndex);
		}

		LoopIndex += 1;
		return;
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDelayEnded.Broadcast(LoopIndex);
	}

	EndTask();
}
