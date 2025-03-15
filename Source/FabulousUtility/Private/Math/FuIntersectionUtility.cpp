#include "Math/FuIntersectionUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuIntersectionUtility)

bool UFuIntersectionUtility::LineSegmentIntersection2D(const FVector2D& LineOrigin, const FVector2D& LineDirection,
                                                       const FVector2D& SegmentStart, const FVector2D& SegmentEnd,
                                                       FVector2D& Intersection)
{
	// https://stackoverflow.com/questions/2931573/determining-if-two-rays-intersect

	const auto SegmentVector{SegmentEnd - SegmentStart};
	const FVector2D SegmentVectorPerpendicular{SegmentVector.Y, -SegmentVector.X};

	const auto Dot{LineDirection | SegmentVectorPerpendicular};
	if (FMath::IsNearlyZero(Dot))
	{
		// The line and the segment are nearly parallel.
		Intersection = FVector2D::ZeroVector;
		return false;
	}

	const auto DotInverse{1.0f / Dot};
	const auto SegmentStartRelative{SegmentStart - LineOrigin};

	// const auto LineTime{(SegmentStartRelative ^ SegmentVector) * DotInverse};
	const auto SegmentTime{(SegmentStartRelative ^ LineDirection) * DotInverse};

	if (SegmentTime < 0.0f || SegmentTime > 1.0f)
	{
		Intersection = FVector2D::ZeroVector;
		return false;
	}

	Intersection = SegmentStart + SegmentVector * SegmentTime;
	return true;
}
