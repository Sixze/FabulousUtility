#pragma once

#include "AbilitySystemGlobals.h"
#include "FuAbilitySystemGlobals.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	static UFuAbilitySystemGlobals& FuGet();

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};

inline UFuAbilitySystemGlobals& UFuAbilitySystemGlobals::FuGet()
{
	return *CastChecked<UFuAbilitySystemGlobals>(IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals());
}
