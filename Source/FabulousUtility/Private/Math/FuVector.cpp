#include "Math/FuVector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuVector)

FVector UFuVector::SlerpSkipNormalization(const FVector& From, const FVector& To, const float Ratio)
{
	// http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/

	auto Dot{From | To};

	if (Dot > 0.9995f)
	{
		return FMath::Lerp(From, To, Ratio).GetSafeNormal();
	}

	Dot = FMath::Max(-1.0f, Dot);

	const auto Theta{UE_REAL_TO_FLOAT(FMath::Acos(Dot)) * Ratio};

	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Theta);

	auto FromPerpendicular{To - From * Dot};
	FromPerpendicular.Normalize();

	return From * Cos + FromPerpendicular * Sin;
}

FVector UFuVector::SlerpByAngleSkipNormalization(const FVector& From, const FVector& To, float Angle)
{
	// http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/

	Angle = FMath::DegreesToRadians(FMath::Max(0.0f, Angle));

	const auto Dot{From | To};

	if (Angle >= FMath::Acos(Dot))
	{
		return To;
	}

	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Angle);

	auto FromPerpendicular{To - From * Dot};
	FromPerpendicular.Normalize();

	return From * Cos + FromPerpendicular * Sin;
}

FVector UFuVector::RandomDirection()
{
	// https://stackoverflow.com/questions/5408276/sampling-uniformly-distributed-random-points-inside-a-spherical-volume

	const auto Phi{FMath::FRandRange(0.0f, UE_TWO_PI)};

	float SinPhi, CosPhi;
	FMath::SinCos(&SinPhi, &CosPhi, Phi);

	const auto Theta{FMath::Acos(FMath::FRandRange(-1.0f, 1.0f))};

	float SinTheta, CosTheta;
	FMath::SinCos(&SinTheta, &CosTheta, Theta);

	return {SinTheta * CosPhi, SinTheta * SinPhi, CosTheta};
}
