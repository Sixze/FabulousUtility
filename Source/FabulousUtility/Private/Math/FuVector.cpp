#include "Math/FuVector.h"

#include "Math/FuMath.h"

FVector UFuVector::SlerpAngleSkipNormalization(const FVector& From, const FVector& To, float Angle)
{
	// http://allenchou.net/2018/05/game-math-deriving-the-slerp-formula/

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

FVector UFuVector::RandomUnitVector()
{
	// http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution

	const auto Phi{FMath::FRandRange(0.0f, UFuMath::TwoPi)};

	float SinPhi, CosPhi;
	FMath::SinCos(&SinPhi, &CosPhi, Phi);

	const auto Theta{FMath::Acos(FMath::FRandRange(-1.0f, 1.0f))};

	float SinTheta, CosTheta;
	FMath::SinCos(&SinTheta, &CosTheta, Theta);

	return {SinTheta * CosPhi, SinTheta * SinPhi, CosTheta};
}
