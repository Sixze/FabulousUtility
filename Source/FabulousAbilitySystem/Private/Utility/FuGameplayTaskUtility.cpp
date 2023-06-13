#include "Utility/FuGameplayTaskUtility.h"

#include "GameplayTask.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayTaskUtility)

void UFuGameplayTaskUtility::TryEndTask(UGameplayTask* Task)
{
	if (IsValid(Task))
	{
		Task->EndTask();
	}
}
