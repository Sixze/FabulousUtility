#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuQuaternion.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuQuaternion : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Quaternion", Meta = (ReturnDisplayName = "Delta"))
	static FQuat DeltaQuaternion(const FQuat& From, const FQuat& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Quaternion", Meta = (ReturnDisplayName = "World"))
	static FQuat TransformRelativeToWorldQuaternion(const FQuat& Relative, const FQuat& Parent);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Quaternion", Meta = (ReturnDisplayName = "Relative"))
	static FQuat TransformWorldToRelativeQuaternion(const FQuat& World, const FQuat& Parent);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Quaternion", Meta = (ReturnDisplayName = "Quaternion"))
	static FQuat AddWorldDeltaQuaternion(const FQuat& Quaternion, const FQuat& WorldDelta);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Quaternion", Meta = (ReturnDisplayName = "Quaternion"))
	static FQuat AddRelativeDeltaQuaternion(const FQuat& Quaternion, const FQuat& RelativeDelta);
};

inline FQuat UFuQuaternion::DeltaQuaternion(const FQuat& From, const FQuat& To)
{
	return To * From.Inverse();
}

inline FQuat UFuQuaternion::TransformRelativeToWorldQuaternion(const FQuat& Relative, const FQuat& Parent)
{
	return Parent * Relative;
}

inline FQuat UFuQuaternion::TransformWorldToRelativeQuaternion(const FQuat& World, const FQuat& Parent)
{
	return Parent.Inverse() * World;
}

inline FQuat UFuQuaternion::AddWorldDeltaQuaternion(const FQuat& Quaternion, const FQuat& WorldDelta)
{
	return WorldDelta * Quaternion;
}

inline FQuat UFuQuaternion::AddRelativeDeltaQuaternion(const FQuat& Quaternion, const FQuat& RelativeDelta)
{
	return TransformRelativeToWorldQuaternion(RelativeDelta, Quaternion);
}
