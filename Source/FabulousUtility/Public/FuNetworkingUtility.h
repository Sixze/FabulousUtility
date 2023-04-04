#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuNetworkingUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuNetworkingUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Networking Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Value"))
	static bool IsActorLocallyControlled(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Networking Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsActorLocallyControlled(const AActor* Actor);
};

inline bool UFuNetworkingUtility::SwitchIsActorLocallyControlled(const AActor* Actor)
{
	return IsActorLocallyControlled(Actor);
}
