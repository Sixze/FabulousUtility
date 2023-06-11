#pragma once

#include "GameplayCueNotify_Actor.h"
#include "FuGameplayCueNotify_Actor.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API AFuGameplayCueNotify_Actor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	virtual void HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
};
