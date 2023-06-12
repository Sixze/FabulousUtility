#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "FuViewportWatermarkSubsystem.generated.h"

UCLASS()
class FABULOUSUI_API UFuViewportWatermarkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
#if !UE_SERVER
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void GameViewport_OnViewportCreated() const;
#endif
};
