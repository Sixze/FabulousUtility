#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuWorldUtility.generated.h"

namespace EWorldType
{
	enum Type;
}

UCLASS()
class FABULOUSUTILITY_API UFuWorldUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static constexpr FStringView WorldTypeToString(EWorldType::Type WorldType);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu World Utility", Meta = (WorldContext = "WorldContext"))
	static float GetWorldGravityZ(const UObject* WorldContext);
};
