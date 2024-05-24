#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuMath.generated.h"

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Value"))
	static float Clamp01(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Interpolation Ammount"))
	static float Damp(float DeltaTime, float Smoothing);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Math Utility", Meta = (ReturnDisplayName = "Interpolation Ammount"))
	static float ExponentialDecay(float DeltaTime, float Lambda);

	template <typename ValueType>
	static ValueType Damp(const ValueType& Current, const ValueType& Target, float DeltaTime, float Smoothing);

	template <typename ValueType>
	static ValueType ExponentialDecay(const ValueType& Current, const ValueType& Target, float DeltaTime, float Lambda);
};

inline float UFuMath::Clamp01(const float Value)
{
	return Value <= 0.0f
		       ? 0.0f
		       : Value >= 1.0f
		       ? 1.0f
		       : Value;
}

inline float UFuMath::Damp(const float DeltaTime, const float Smoothing)
{
	// https://www.rorydriscoll.com/2016/03/07/frame-rate-independent-damping-using-lerp/

	return 1.0f - FMath::Pow(Smoothing, DeltaTime);
}

inline float UFuMath::ExponentialDecay(const float DeltaTime, const float Lambda)
{
	// https://www.rorydriscoll.com/2016/03/07/frame-rate-independent-damping-using-lerp/

	return 1.0f - FMath::InvExpApprox(Lambda * DeltaTime);
}

template <typename ValueType>
ValueType UFuMath::Damp(const ValueType& Current, const ValueType& Target, const float DeltaTime, const float Smoothing)
{
	return Smoothing > 0.0f
		       ? FMath::Lerp(Current, Target, Damp(DeltaTime, Smoothing))
		       : Target;
}

template <typename ValueType>
ValueType UFuMath::ExponentialDecay(const ValueType& Current, const ValueType& Target, const float DeltaTime, const float Lambda)
{
	return Lambda > 0.0f
		       ? FMath::Lerp(Current, Target, ExponentialDecay(DeltaTime, Lambda))
		       : Target;
}
