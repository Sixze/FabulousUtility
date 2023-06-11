#include "FuAbilitySystemGlobalsSubsystem.h"

#include "AbilitySystemGlobals.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilitySystemGlobalsSubsystem)

void UFuAbilitySystemGlobalsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
