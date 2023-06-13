#include "GameplayCue/FuGameplayCueManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayCueManager)

bool UFuGameplayCueManager::ShouldAsyncLoadRuntimeObjectLibraries() const
{
	// https://github.com/tranek/GASDocumentation#concepts-gc-manager

	return false;
}
