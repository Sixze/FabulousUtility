#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuVector.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuVector : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "Direction", ReturnDisplayName = "Angle"))
	static double DirectionToAngle(const FVector2D& Direction);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "Direction", ReturnDisplayName = "Angle"))
	static double DirectionToAngleXY(const FVector& Direction);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Angle"))
	static double AngleBetween(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Angle"))
	static double AngleBetweenXY(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Angle"))
	static double AngleBetweenSignedXY(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility", DisplayName = "Angle Between (Skip Normalization)",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Angle"))
	static double AngleBetweenSkipNormalization(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility", DisplayName = "Slerp (Skip Normalization)",
		Meta = (AutoCreateRefTerm = "From, To", ReturnDisplayName = "Direction"))
	static FVector SlerpSkipNormalization(const FVector& From, const FVector& To, float Angle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility", Meta = (ReturnDisplayName = "Direction"))
	static FVector RandomDirection();

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "SegmentStart, SegmentEnd, Point", ReturnDisplayName = "Point"))
	static FVector ClosestPointOnSegmentXY(const FVector& SegmentStart, const FVector& SegmentEnd, const FVector& Point);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Vector Utility",
		Meta = (AutoCreateRefTerm = "SegmentStart, SegmentEnd, Point", ReturnDisplayName = "Distance"))
	static double DistanceToSegmentXY(const FVector& SegmentStart, const FVector& SegmentEnd, const FVector& Point);
};

inline double UFuVector::DirectionToAngle(const FVector2D& Direction)
{
	return FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
}

inline double UFuVector::DirectionToAngleXY(const FVector& Direction)
{
	return FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
}

inline double UFuVector::AngleBetween(const FVector& From, const FVector& To)
{
	return FMath::RadiansToDegrees(FMath::Acos(From.GetSafeNormal() | To.GetSafeNormal()));
}

inline double UFuVector::AngleBetweenXY(const FVector& From, const FVector& To)
{
	return FMath::RadiansToDegrees(FMath::Acos(From.GetSafeNormal2D() | To.GetSafeNormal2D()));
}

inline double UFuVector::AngleBetweenSignedXY(const FVector& From, const FVector& To)
{
	const auto FromXY{FVector2D{From}.GetSafeNormal()};
	const auto ToXY{FVector2D{To}.GetSafeNormal()};

	// return FMath::RadiansToDegrees(FMath::Atan2(FromXY ^ ToXY, FromXY | ToXY));

	return FMath::RadiansToDegrees(FMath::Acos(FromXY | ToXY)) * FMath::Sign(FromXY ^ ToXY);
}

inline double UFuVector::AngleBetweenSkipNormalization(const FVector& From, const FVector& To)
{
	return FMath::RadiansToDegrees(FMath::Acos(From | To));
}

inline FVector UFuVector::ClosestPointOnSegmentXY(const FVector& SegmentStart, const FVector& SegmentEnd, const FVector& Point)
{
	const auto ClosestPoint{FMath::ClosestPointOnSegment2D(FVector2D{Point}, FVector2D{SegmentStart}, FVector2D{SegmentEnd})};

	return {ClosestPoint.X, ClosestPoint.Y, 0.0f};
}

inline double UFuVector::DistanceToSegmentXY(const FVector& SegmentStart, const FVector& SegmentEnd, const FVector& Point)
{
	return FVector2D::Distance(FVector2D{Point},
	                           FMath::ClosestPointOnSegment2D(FVector2D{Point}, FVector2D{SegmentStart}, FVector2D{SegmentEnd}));
}
