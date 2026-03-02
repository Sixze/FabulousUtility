#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "FuSlateNavigationSubsystem.generated.h"

UCLASS()
class FABULOUSUI_API UFuSlateNavigationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#if !UE_SERVER
public:
	virtual bool ShouldCreateSubsystem(UObject* OuterObject) const override;

	virtual void Initialize(FSubsystemCollectionBase& SubsystemCollection) override;

	virtual void Deinitialize() override;
#endif
};
