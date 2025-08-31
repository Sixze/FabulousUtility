#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuQuaternion.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuQuaternion : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility", Meta = (ReturnDisplayName = "Delta"))
	static FQuat DeltaQuaternion(const FQuat& From, const FQuat& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility", Meta = (ReturnDisplayName = "World"))
	static FQuat TransformRelativeToWorldQuaternion(const FQuat& Relative, const FQuat& Parent);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility", Meta = (ReturnDisplayName = "Relative"))
	static FQuat TransformWorldToRelativeQuaternion(const FQuat& World, const FQuat& Parent);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility", Meta = (ReturnDisplayName = "Quaternion"))
	static FQuat AddWorldDeltaQuaternion(const FQuat& Quaternion, const FQuat& WorldDelta);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility", Meta = (ReturnDisplayName = "Quaternion"))
	static FQuat AddRelativeDeltaQuaternion(const FQuat& Quaternion, const FQuat& RelativeDelta);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility",
		Meta = (AutoCreateRefTerm = "TwistAxis", ReturnDisplayName = "Twist"))
	static FQuat GetTwist(const FQuat& Quaternion, const FVector& TwistAxis = FVector::UpVector);

	// Same as FQuat::FindBetweenVectors(FVector::ForwardVector, Vector), but faster. Does not preserve
	// the up vector (roll can be non-zero), so if it is important, use FVector::ToOrientationQuat() instead.
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility",
		Meta = (AutoCreateRefTerm = "Vector", ReturnDisplayName = "Quaternion"))
	static FQuat VectorToQuaternion(const FVector& Vector);

	// Same as FQuat::FindBetweenNormals(FVector::ForwardVector, Vector), but faster. Does not preserve
	// the up vector (roll can be non-zero), so if it is important, use FVector::ToOrientationQuat() instead.
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Quaternion Utility",
		Meta = (AutoCreateRefTerm = "Direction", ReturnDisplayName = "Quaternion"))
	static FQuat DirectionToQuaternion(const FVector& Direction);

	static FQuat VectorToQuaternion(const FVector& Vector, double VectorLength);
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
	return Quaternion * RelativeDelta;
}

inline FQuat UFuQuaternion::GetTwist(const FQuat& Quaternion, const FVector& TwistAxis)
{
	// Based on TQuat<T>::ToSwingTwist().

	const auto Projection{(TwistAxis | FVector{Quaternion.X, Quaternion.Y, Quaternion.Z}) * TwistAxis};

	return FQuat{Projection.X, Projection.Y, Projection.Z, Quaternion.W}.GetNormalized();
}

inline FQuat UFuQuaternion::VectorToQuaternion(const FVector& Vector)
{
	return VectorToQuaternion(Vector, Vector.Size());
}

inline FQuat UFuQuaternion::DirectionToQuaternion(const FVector& Direction)
{
	return VectorToQuaternion(Direction, 1.0f);
}
