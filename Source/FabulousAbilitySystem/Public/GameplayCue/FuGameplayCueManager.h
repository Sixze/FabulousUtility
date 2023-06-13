#pragma once

#include "GameplayCueManager.h"
#include "FuGameplayCueManager.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

protected:
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
};
