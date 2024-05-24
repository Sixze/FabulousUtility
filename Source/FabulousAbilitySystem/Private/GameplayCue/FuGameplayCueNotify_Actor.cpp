#include "GameplayCue/FuGameplayCueNotify_Actor.h"

#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayCueNotify_Actor)

void AFuGameplayCueNotify_Actor::HandleGameplayCue(AActor* Target, const EGameplayCueEvent::Type EventType,
                                                   const FGameplayCueParameters& Parameters)
{
	if (EventType != EGameplayCueEvent::Removed)
	{
		// This fixes an issue where OnRemove() was never called again after this cue was re-triggered during its removal. For
		// example, the user has configured this cue so that it does not end immediately after OnRemove() is called, but with some
		// delay, after which GameplayCueFinishedCallback() is manually called, and if this cue is re-triggered during this delay,
		// the bHasHandledOnRemoveEvent variable will not be reset, and therefore the cue will never be able to end normally again.

		// TODO Wait for https://github.com/EpicGames/UnrealEngine/pull/11046 to be merged into the engine.

		bHasHandledOnRemoveEvent = false;
		GetWorldTimerManager().ClearTimer(FinishTimerHandle);
	}

	Super::HandleGameplayCue(Target, EventType, Parameters);
}
