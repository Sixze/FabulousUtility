#include "AbilitySystem/FuAbilitySystemGlobals.h"

#include "AbilitySystem/FuGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilitySystemGlobals)

FGameplayEffectContext* UFuAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FFuGameplayEffectContext{};
}
