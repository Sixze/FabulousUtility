#include "FuGameplayTaskUtility.h"

#include "GameplayTask.h"

void UFuGameplayTaskUtility::TryEndTask(UGameplayTask* Task)
{
	if (IsValid(Task))
	{
		Task->EndTask();
	}
}
