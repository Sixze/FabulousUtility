#include "AbilitySystem/AbilityTasks/FuAbilityTask_Tick.h"

UFuAbilityTask_Tick::UFuAbilityTask_Tick()
{
	bTickingTask = true;
}

UFuAbilityTask_Tick* UFuAbilityTask_Tick::FuWaitForTick(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<ThisClass>(OwningAbility);
}

void UFuAbilityTask_Tick::TickTask(const float DeltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}
