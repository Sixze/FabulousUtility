#include "FuAsyncActionUtility.h"

#include "Engine/CancellableAsyncAction.h"

void UFuAsyncActionUtility::TryCancelAction(UCancellableAsyncAction* Action)
{
	if (IsValid(Action))
	{
		Action->Cancel();
	}
}
