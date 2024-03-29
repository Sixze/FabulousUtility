#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAudioUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAudioUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Audio Utility", Meta = (WorldContext = "WorldContext"))
	static void SetTransientPrimaryVolume(const UObject* WorldContext, float Volume);
};
