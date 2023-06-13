#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSessionUtility.generated.h"

UCLASS()
class FABULOUSONLINE_API UFuSessionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session Utility",
		Meta = (DefaultToSelf = "Player", ReturnDisplayName = "Success"))
	static bool TravelToGameSession(APlayerController* Player);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Session Utility",
		Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryTravelToGameSession(APlayerController* Player);
};

inline bool UFuSessionUtility::TryTravelToGameSession(APlayerController* Player)
{
	return TravelToGameSession(Player);
}
