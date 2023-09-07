#include "AbilityTasks/FuAbilityTask_Tick.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_Tick)

UFuAbilityTask_Tick::UFuAbilityTask_Tick()
{
	bTickingTask = true;
}

UFuAbilityTask_Tick* UFuAbilityTask_Tick::WaitForTick(UGameplayAbility* OwningAbility)
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
