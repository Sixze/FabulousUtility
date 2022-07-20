#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "FuAbilityGlobalsSubsystem.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilityGlobalsSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
