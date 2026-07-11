#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuMath.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename ValueType> requires UE::CFloatingPoint<ValueType>
	static constexpr ValueType SmoothMax(ValueType A, ValueType B, float Lambda);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Value"))
	static float Clamp01(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Alpha"))
	static float DamperExactAlpha(float DeltaTime, float HalfLife);

	/// HalfLife is the time it takes for the distance to the target to be reduced by half.
	template <typename ValueType>
	static ValueType DamperExact(const ValueType& Current, const ValueType& Target, float DeltaTime, float HalfLife);
};

template <typename ValueType> requires UE::CFloatingPoint<ValueType>
constexpr ValueType UFuMath::SmoothMax(const ValueType A, const ValueType B, const float Lambda)
{
	// https://www.youtube.com/watch?v=6Qb6QtC6QMs

	return (A + B + FMath::Sqrt(FMath::Square(A - B) + FMath::Max(0.0f, Lambda))) / 2.0f;
}

inline float UFuMath::Clamp01(const float Value)
{
	return Value > 0.0f
		       ? Value < 1.0f
			         ? Value
			         : 1.0f
		       : 0.0f;
}

inline float UFuMath::DamperExactAlpha(const float DeltaTime, const float HalfLife)
{
	// https://theorangeduck.com/page/spring-roll-call#exactdamper

	return 1.0f - FMath::InvExpApprox(UE_LN2 * DeltaTime / (HalfLife + UE_SMALL_NUMBER));
}

template <typename ValueType>
ValueType UFuMath::DamperExact(const ValueType& Current, const ValueType& Target, const float DeltaTime, const float HalfLife)
{
	return FMath::Lerp(Current, Target, DamperExactAlpha(DeltaTime, HalfLife));
}
