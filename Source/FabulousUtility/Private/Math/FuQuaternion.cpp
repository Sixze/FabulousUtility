#include "Math/FuQuaternion.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuQuaternion)

FQuat UFuQuaternion::VectorToQuaternion(const FVector& Vector, const double VectorLength)
{
	// Based on UE::Math::FindBetween_Helper(), where A is FVector::ForwardVector.

	const auto W{VectorLength + Vector.X};

	if (W < 1e-6f * VectorLength)
	{
		// Vector points backwards.
		return {0.0f, 0.0f, 1.0f, 0.0f};
	}

	FQuat Result{0.0f, -Vector.Z, Vector.Y, W};
	Result.Normalize();

	return Result;
}
