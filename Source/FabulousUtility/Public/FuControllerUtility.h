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
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Value"))
	static bool HasController(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchHasController(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Controller"))
	static AController* GetControllerFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetControllerFromActor(AActor* Actor, AController*& Controller);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Value"))
	static bool HasPlayerController(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchHasPlayerController(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Controller"))
	static APlayerController* GetPlayerControllerFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetPlayerControllerFromActor(AActor* Actor, APlayerController*& Player);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Controller Utility",
		Meta = (DefaultToSelf = "Player", ReturnDisplayName = "Local Player"))
	static ULocalPlayer* GetLocalPlayer(const APlayerController* Player);
};

inline bool UFuControllerUtility::HasController(AActor* Actor)
{
	return IsValid(GetControllerFromActor(Actor));
}

inline bool UFuControllerUtility::SwitchHasController(AActor* Actor)
{
	return HasController(Actor);
}

inline bool UFuControllerUtility::HasPlayerController(AActor* Actor)
{
	return IsValid(GetPlayerControllerFromActor(Actor));
}

inline bool UFuControllerUtility::SwitchHasPlayerController(AActor* Actor)
{
	return HasPlayerController(Actor);
}

inline APlayerController* UFuControllerUtility::GetPlayerControllerFromActor(AActor* Actor)
{
	return Cast<APlayerController>(GetControllerFromActor(Actor));
}

inline bool UFuControllerUtility::TryGetPlayerControllerFromActor(AActor* Actor, APlayerController*& Player)
{
	AController* Controller;
	TryGetControllerFromActor(Actor, Controller);

	Player = Cast<APlayerController>(Controller);
	return IsValid(Player);
}

inline ULocalPlayer* UFuControllerUtility::GetLocalPlayer(const APlayerController* Player)
{
	return Player->GetLocalPlayer();
}
