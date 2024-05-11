#include "Math/FuQuaternion.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuQuaternion)

FQuat UFuQuaternion::GetTwist(const FQuat& Quaternion, const FVector& TwistAxis)
{
	// Based on TQuat<T>::ToSwingTwist().

	const auto Projection{(TwistAxis | FVector{Quaternion.X, Quaternion.Y, Quaternion.Z}) * TwistAxis};

	return FQuat{Projection.X, Projection.Y, Projection.Z, Quaternion.W}.GetNormalized();
}
