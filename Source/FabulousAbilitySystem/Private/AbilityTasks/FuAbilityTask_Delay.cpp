#include "AbilityTasks/FuAbilityTask_Delay.h"

#include "AbilitySystemGlobals.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_Delay)

UFuAbilityTask_Delay* UFuAbilityTask_Delay::FuWaitForDelay(UGameplayAbility* OwningAbility, float InDuration,
                                                           const int32 InLoopsCount, const bool bInSkipFirstDelay)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(InDuration);

	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->Duration = FMath::Max(0.0f, InDuration);
	Task->LoopsCount = FMath::Max(-1, InLoopsCount);
	Task->bSkipFirstDelay = bInSkipFirstDelay;

	return Task;
}

void UFuAbilityTask_Delay::Activate()
{
	Super::Activate();

	if (Duration <= UE_SMALL_NUMBER)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			for (auto i{0}; i < LoopsCount; i++)
			{
				LoopIndex += 1;
				OnLoop.Broadcast(LoopIndex);
			}

			OnDelayEnded.Broadcast(FMath::Max(LoopIndex + 1, LoopsCount));
		}

		EndTask();
		return;
	}

	if (bSkipFirstDelay)
	{
		TimerManager_OnTimerEnded();
	}

	if (!IsFinished())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::TimerManager_OnTimerEnded, Duration, LoopsCount != 0);
	}
}

FString UFuAbilityTask_Delay::GetDebugString() const
{
	TStringBuilder<256> DebugStringBuilder;

	DebugStringBuilder << GetFName() << TEXTVIEW(" (") << InstanceName << TEXTVIEW("): Time Remaining: ");
	DebugStringBuilder.Appendf(TEXT("%.2f"), Duration - GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle));
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

	if (LoopsCount < 0 || LoopIndex < LoopsCount)
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
