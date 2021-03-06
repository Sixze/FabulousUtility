#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "FuViewportWatermarkSubsystem.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuViewportWatermarkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#if !UE_SERVER
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void OnViewportCreated() const;
#endif
};
