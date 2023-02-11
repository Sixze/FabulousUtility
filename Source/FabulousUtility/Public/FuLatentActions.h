#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuLatentActions.generated.h"

UENUM(BlueprintType)
enum class EFuDelayInputExecs : uint8
{
	Start,
	Stop
};

UENUM(BlueprintType)
enum class EFuDelayOutputExecs : uint8
{
	OnLoop,
	OnDelayEnded
};

UCLASS()
class FABULOUSUTILITY_API UFuLatentActions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Fabulous Utility|Fu Latent Actions", Meta = (WorldContext = "WorldContext",
		Latent, LatentInfo = "LatentInfo", ExpandEnumAsExecs = "Input, Output", Duration = 0.2))
	static void FuDelay(const UObject* WorldContext, FLatentActionInfo LatentInfo, EFuDelayInputExecs Input, float Duration,
	                    int32 LoopsCount, bool bSkipFirstDelay, bool bRetriggerable, int32& LoopIndex, EFuDelayOutputExecs& Output);
};
