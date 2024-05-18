#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "FuViewportWatermarkSubsystem.generated.h"

UCLASS()
class FABULOUSUI_API UFuViewportWatermarkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
#if !UE_SERVER
	virtual bool ShouldCreateSubsystem(UObject* OuterObject) const override;

	virtual void Initialize(FSubsystemCollectionBase& SubsystemCollection) override;

	virtual void Deinitialize() override;

private:
	void GameViewport_OnViewportCreated() const;
#endif
};
