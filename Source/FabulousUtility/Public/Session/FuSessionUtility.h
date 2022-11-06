#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSessionUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuSessionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session Utility", Meta = (DefaultToSelf = "Player"))
	static bool TryTravelToGameSession(APlayerController* Player);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session Utility",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchTryTravelToGameSession(APlayerController* Player);
};

inline bool UFuSessionUtility::SwitchTryTravelToGameSession(APlayerController* Player)
{
	return TryTravelToGameSession(Player);
}
