#include "Math/FuVector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuVector)

FVector UFuVector::SlerpSkipNormalization(const FVector& From, const FVector& To, float Angle)
{
	// https://allenchou.net/2018/05/game-math-deriving-the-slerp-formula/

	Angle = FMath::DegreesToRadians(FMath::Max(0.0f, Angle));

	const auto Dot{From | To};

	if (Angle >= FMath::Acos(Dot))
	{
		return To;
	}

	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Angle);

	const auto FromPerpendicular{(To - From * Dot).GetSafeNormal()};

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
