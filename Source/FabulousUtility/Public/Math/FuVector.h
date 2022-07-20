#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuVector.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuVector : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector", Meta = (AutoCreateRefTerm = "Direction"))
	static double DirectionToAngle(const FVector2D& Direction);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector", Meta = (AutoCreateRefTerm = "Direction"))
	static double DirectionToAngleXY(const FVector& Direction);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector", Meta = (AutoCreateRefTerm = "From, To"))
	static double AngleBetween(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector",
		DisplayName = "Angle Between (Skip Normalization)", Meta = (AutoCreateRefTerm = "From, To"))
	static double AngleBetweenSkipNormalization(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector",
		DisplayName = "Slerp Angle (Skip Normalization)", Meta = (AutoCreateRefTerm = "From, To"))
	static FVector SlerpAngleSkipNormalization(const FVector& From, const FVector& To, float Angle);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector")
	static FVector RandomUnitVector();

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector", Meta = (AutoCreateRefTerm = "SegmentStart, SegmentEnd, Point"))
	static FVector ClosestPointOnSegmentXY(const FVector& SegmentStart, const FVector& SegmentEnd, const FVector& Point);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector", Meta = (AutoCreateRefTerm = "SegmentStart, SegmentEnd, Point"))
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
