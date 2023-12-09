#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuLatentActions.generated.h"

UENUM(BlueprintType)
enum class EFuAdvancedDelayInputExecs : uint8
{
	Start,
	Stop
};

UENUM(BlueprintType)
enum class EFuAdvancedDelayOutputExecs : uint8
{
	OnLoop,
	OnDelayEnded
};

UCLASS()
class FABULOUSUTILITY_API UFuLatentActions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Latent Actions", DisplayName = "Delay (Advanced)",
		Meta = (WorldContext = "WorldContext", Latent, LatentInfo = "LatentInfo", ExpandEnumAsExecs = "Input, Output", Duration = 0.2))
	static void AdvancedDelay(const UObject* WorldContext, FLatentActionInfo LatentInfo, EFuAdvancedDelayInputExecs Input,
	                          float Duration, int32 LoopsCount, bool bSkipFirstDelay, bool bRetriggerable,
	                          int32& LoopIndex, EFuAdvancedDelayOutputExecs& Output);
};
