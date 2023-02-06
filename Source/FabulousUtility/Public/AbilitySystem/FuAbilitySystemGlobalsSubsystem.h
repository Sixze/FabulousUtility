#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "FuAbilitySystemGlobalsSubsystem.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilitySystemGlobalsSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
