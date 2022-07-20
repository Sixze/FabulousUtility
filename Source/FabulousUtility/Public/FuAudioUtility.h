#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAudioUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAudioUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Fabulous Utility|Fu Audio Utility", Meta = (WorldContext = "WorldContext"))
	static void SetTransientMasterVolume(const UObject* WorldContext, float Volume);
};
