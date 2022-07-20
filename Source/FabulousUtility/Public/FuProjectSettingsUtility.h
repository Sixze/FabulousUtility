#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuProjectSettingsUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuProjectSettingsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Project Settings Utility")
	static FString GetProjectVersion();

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Project Settings Utility")
	static FString GetCopyrightNotice();
};
