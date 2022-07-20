#include "AbilitySystem/FuAbilitySystemGlobals.h"

#include "AbilitySystem/FuGameplayEffectContext.h"

FGameplayEffectContext* UFuAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FFuGameplayEffectContext{};
}
