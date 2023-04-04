#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Utility",
		Meta = (WorldContext = "WorldContext", DeterminesOutputType = "GameInstanceClass", ReturnDisplayName = "Game Instance"))
	static UGameInstance* GetGameInstanceCasted(const UObject* WorldContext, TSubclassOf<UGameInstance> GameInstanceClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Utility", Meta = (WorldContext = "WorldContext",
		DeterminesOutputType = "GameInstanceClass", DynamicOutputParam = "GameInstance", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetGameInstanceCasted(const UObject* WorldContext, TSubclassOf<UGameInstance> GameInstanceClass,
	                                     UGameInstance*& GameInstance);
};
