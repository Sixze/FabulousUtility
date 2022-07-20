#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuNetworkingUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuNetworkingUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Networking Utility", Meta = (DefaultToSelf = "Actor"))
	static bool IsLocallyControlled(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Networking Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsLocallyControlled(const AActor* Actor);
};

inline bool UFuNetworkingUtility::SwitchIsLocallyControlled(const AActor* Actor)
{
	return IsLocallyControlled(Actor);
}
