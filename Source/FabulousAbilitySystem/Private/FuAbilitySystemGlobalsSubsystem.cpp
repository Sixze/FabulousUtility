#include "FuAbilitySystemGlobalsSubsystem.h"

#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilitySystemGlobalsSubsystem)

void UFuAbilitySystemGlobalsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
