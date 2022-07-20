#pragma once

#include "Engine/LatentActionManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuLatentActions.generated.h"

UENUM(BlueprintType)
enum class EFuDelayStatus : uint8
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
		Latent, LatentInfo = "LatentInfo", ExpandEnumAsExecs = "Status", Duration = 0.2, LoopCount = 1))
	static void FuDelay(const UObject* WorldContext, FLatentActionInfo LatentInfo, float Duration, int32 LoopsCount,
	                    bool bSkipFirstDelay, bool bRetriggerable, int32& LoopIndex, EFuDelayStatus& Status);
};
