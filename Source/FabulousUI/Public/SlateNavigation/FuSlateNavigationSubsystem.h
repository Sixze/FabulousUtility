#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "FuSlateNavigationSubsystem.generated.h"

UCLASS()
class FABULOUSUI_API UFuSlateNavigationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
#if !UE_SERVER
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
#endif
};
