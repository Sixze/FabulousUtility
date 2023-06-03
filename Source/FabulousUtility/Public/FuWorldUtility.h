#pragma once

#include "Engine/EngineTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuWorldUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuWorldUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static constexpr FStringView WorldTypeToString(EWorldType::Type WorldType);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu World Utility",
		Meta = (WorldContext = "WorldContext", ReturnDisplayName = "Gravity Z"))
	static float GetWorldGravityZ(const UObject* WorldContext);
};
