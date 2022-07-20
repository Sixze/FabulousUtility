#include "AbilitySystem/GameplayCue/FuGameplayCueManager.h"

bool UFuGameplayCueManager::ShouldAsyncLoadRuntimeObjectLibraries() const
{
	// https://github.com/tranek/GASDocumentation#concepts-gc-manager

	return false;
}
