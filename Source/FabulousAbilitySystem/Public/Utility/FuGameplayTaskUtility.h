#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuGameplayTaskUtility.generated.h"

class UGameplayTask;

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuGameplayTaskUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Gameplay Task Utility")
	static void TryEndTask(UGameplayTask* Task);
};
