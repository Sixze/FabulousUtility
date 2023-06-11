#include "AbilityTasks/FuAbilityTask_OneFrameDelay.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_OneFrameDelay)

UFuAbilityTask_OneFrameDelay* UFuAbilityTask_OneFrameDelay::FuWaitForOneFrameDelay(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<ThisClass>(OwningAbility);
}

void UFuAbilityTask_OneFrameDelay::Activate()
{
	Super::Activate();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TimerManager_OnTimerEnded);
}

void UFuAbilityTask_OneFrameDelay::OnDestroy(const bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_OneFrameDelay::TimerManager_OnTimerEnded()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDelayEnded.Broadcast();
	}

	EndTask();
}
