#pragma once

#include "Subsystems/EngineSubsystem.h"
#include "FuAbilitySystemGlobalsSubsystem.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilitySystemGlobalsSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& SubsystemCollection) override;
};
