#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuSessionUtility.generated.h"

UCLASS()
class FABULOUSONLINE_API UFuSessionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Online|Session Utility",
		Meta = (DefaultToSelf = "Player", ReturnDisplayName = "Success"))
	static bool TravelToGameSession(APlayerController* Player);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Online|Session Utility",
		DisplayName = "Travel to Game Session (Expanded)", Meta = (DefaultToSelf = "Player", ExpandBoolAsExecs = "ReturnValue"))
	static bool TravelToGameSessionExpanded(APlayerController* Player);
};

inline bool UFuSessionUtility::TravelToGameSessionExpanded(APlayerController* Player)
{
	return TravelToGameSession(Player);
}
