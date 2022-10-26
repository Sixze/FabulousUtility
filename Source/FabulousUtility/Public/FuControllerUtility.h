#pragma once

#include "GameFramework/PlayerController.h"
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

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetController(AActor* Actor, AController*& Controller);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Actor"))
	static bool HasPlayerController(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchHasPlayerController(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Actor"))
	static APlayerController* GetPlayerController(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetPlayerController(AActor* Actor, APlayerController*& Player);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility", Meta = (DefaultToSelf = "Player"))
	static ULocalPlayer* GetLocalPlayer(const APlayerController* Player);
};

inline bool UFuControllerUtility::HasController(AActor* Actor)
{
	return IsValid(GetController(Actor));
}

inline bool UFuControllerUtility::SwitchHasController(AActor* Actor)
{
	return HasController(Actor);
}

inline bool UFuControllerUtility::HasPlayerController(AActor* Actor)
{
	return IsValid(GetPlayerController(Actor));
}

inline bool UFuControllerUtility::SwitchHasPlayerController(AActor* Actor)
{
	return HasPlayerController(Actor);
}

inline ULocalPlayer* UFuControllerUtility::GetLocalPlayer(const APlayerController* Player)
{
	return Player->GetLocalPlayer();
}
