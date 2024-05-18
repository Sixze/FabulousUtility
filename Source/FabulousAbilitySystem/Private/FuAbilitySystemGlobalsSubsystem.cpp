#include "FuAbilitySystemGlobalsSubsystem.h"

#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilitySystemGlobalsSubsystem)

void UFuAbilitySystemGlobalsSubsystem::Initialize(FSubsystemCollectionBase& SubsystemCollection)
{
	Super::Initialize(SubsystemCollection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
