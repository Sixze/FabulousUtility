#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAsyncActionUtility.generated.h"

class UCancellableAsyncAction;

UCLASS()
class FABULOUSUTILITY_API UFuAsyncActionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Async Action Utility")
	static void TryCancelAction(UCancellableAsyncAction* Action);
};
