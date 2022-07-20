#include "AbilitySystem/FuAbilityGlobalsSubsystem.h"

#include "AbilitySystemGlobals.h"

void UFuAbilityGlobalsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
