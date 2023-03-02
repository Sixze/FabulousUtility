#include "AbilitySystem/AbilityTasks/FuAbilityTask_Delay.h"

#include "AbilitySystemGlobals.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_Delay)

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

	if (Duration1 <= UE_SMALL_NUMBER)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			for (auto i{0}; i < LoopsCount1; i++)
			{
				LoopIndex += 1;
				OnLoop.Broadcast(LoopIndex);
			}

			OnDelayEnded.Broadcast(FMath::Max(LoopIndex + 1, LoopsCount1));
		}

		EndTask();
		return;
	}

	if (bSkipFirstDelay1)
	{
		TimerManager_OnTimerEnded();
	}

	if (!IsFinished())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::TimerManager_OnTimerEnded, Duration1, LoopsCount1 != 0);
	}
}

FString UFuAbilityTask_Delay::GetDebugString() const
{
	TStringBuilder<256> DebugStringBuilder;

	DebugStringBuilder << GetFName() << TEXTVIEW(" (") << InstanceName << TEXTVIEW("): Time Remaining: ");
	DebugStringBuilder.Appendf(TEXT("%.2f"), Duration1 - GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle));
	DebugStringBuilder << TEXTVIEW(", Loop Index: ") << LoopIndex;

	return FString{DebugStringBuilder};
}

void UFuAbilityTask_Delay::OnDestroy(const bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_Delay::TimerManager_OnTimerEnded()
{
	if (IsFinished())
	{
		return;
	}

	LoopIndex += 1;

	if (LoopsCount1 < 0 || LoopIndex < LoopsCount1)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnLoop.Broadcast(LoopIndex);
		}

		return;
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDelayEnded.Broadcast(LoopIndex);
	}

	EndTask();
}
