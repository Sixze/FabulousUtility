#pragma once

#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "FuObjectUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuObjectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Object Utility", Meta = (DeterminesOutputType = "Class"))
	static UObject* GetDefaultObject(TSubclassOf<UObject> Class);
};

inline UObject* UFuObjectUtility::GetDefaultObject(const TSubclassOf<UObject> Class)
{
	return FU_ENSURE(IsValid(Class)) ? Class.GetDefaultObject() : nullptr;
}
