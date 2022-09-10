#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "FuSlateNavigationSubsystem.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuSlateNavigationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#if !UE_SERVER
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
#endif
};
