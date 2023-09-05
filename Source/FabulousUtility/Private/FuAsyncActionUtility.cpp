#include "FuAsyncActionUtility.h"

#include "Engine/CancellableAsyncAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncActionUtility)

void UFuAsyncActionUtility::CancelActionSafe(UCancellableAsyncAction* Action)
{
	if (IsValid(Action))
	{
		Action->Cancel();
	}
}
