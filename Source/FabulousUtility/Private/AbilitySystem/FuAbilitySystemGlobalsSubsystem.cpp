#include "AbilitySystem/FuAbilitySystemGlobalsSubsystem.h"

#include "AbilitySystemGlobals.h"

void UFuAbilitySystemGlobalsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
