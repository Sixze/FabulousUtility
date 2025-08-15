#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuMath.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static constexpr auto Ln2{0.6931471805599453f}; // FMath::Loge(2.0f).

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Value"))
	static float Clamp01(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Alpha"))
	static float DamperExactAlpha(float DeltaTime, float HalfLife);

	// HalfLife is the time it takes for the distance to the target to be reduced by half.
	template <typename ValueType>
	static ValueType DamperExact(const ValueType& Current, const ValueType& Target, float DeltaTime, float HalfLife);
};

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

	return 1.0f - FMath::InvExpApprox(Ln2 * DeltaTime / (HalfLife + UE_SMALL_NUMBER));
}

template <typename ValueType>
ValueType UFuMath::DamperExact(const ValueType& Current, const ValueType& Target, const float DeltaTime, const float HalfLife)
{
	return FMath::Lerp(Current, Target, DamperExactAlpha(DeltaTime, HalfLife));
}
