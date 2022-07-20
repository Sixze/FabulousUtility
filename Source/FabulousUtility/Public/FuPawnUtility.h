#pragma once

#include "FuMacros.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuPawnUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuPawnUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Pawn Utility", Meta = (DefaultToSelf = "Pawn"))
	static APlayerController* GetPlayerController(const APawn* Pawn);
};

inline APlayerController* UFuPawnUtility::GetPlayerController(const APawn* Pawn)
{
	return FU_ENSURE(IsValid(Pawn)) ? Cast<APlayerController>(Pawn->Controller) : nullptr;
}
