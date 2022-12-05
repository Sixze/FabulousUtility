#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Utility")
	static FString GetObjectName(const TSoftObjectPtr<UObject> SoftObjectReference);
};
