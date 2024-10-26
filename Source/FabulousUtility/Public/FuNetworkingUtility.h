#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuNetworkingUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuNetworkingUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Networking Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Value"))
	static bool IsActorLocallyControlled(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Networking Utility",
		DisplayName = "Is Actor Locally Controlled (Expanded)", Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool IsActorLocallyControlledExpanded(const AActor* Actor);
};

inline bool UFuNetworkingUtility::IsActorLocallyControlledExpanded(const AActor* Actor)
{
	return IsActorLocallyControlled(Actor);
}
