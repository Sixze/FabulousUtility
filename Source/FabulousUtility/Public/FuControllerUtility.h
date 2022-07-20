#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuControllerUtility.generated.h"

class AController;

UCLASS()
class FABULOUSUTILITY_API UFuControllerUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Actor"))
	static bool HasController(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchHasController(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Actor"))
	static AController* GetController(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Actor"))
	static bool HasPlayerController(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchHasPlayerController(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Actor"))
	static APlayerController* GetPlayerController(AActor* Actor);
};

inline bool UFuControllerUtility::SwitchHasController(AActor* Actor)
{
	return HasController(Actor);
}

inline bool UFuControllerUtility::SwitchHasPlayerController(AActor* Actor)
{
	return HasPlayerController(Actor);
}
