#pragma once

#include "InputCoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuCommonInputUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuCommonInputUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Common Input Utility")
	static ECommonInputType GetInputTypeForKey(const FKey& Key);
};
