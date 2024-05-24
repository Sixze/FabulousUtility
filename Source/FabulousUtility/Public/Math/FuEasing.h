#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEasing.generated.h"

// https://github.com/warrenm/AHEasing

UCLASS(Meta = (BlueprintThreadSafe))
class FABULOUSUTILITY_API UFuEasing : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Sine Wave Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float SineEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float SineEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float SineEaseInOut(float Value);

	// Quadratic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuadraticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuadraticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuadraticEaseInOut(float Value);

	// Cubic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float CubicEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float CubicEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float CubicEaseInOut(float Value);

	// Quartic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuarticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuarticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuarticEaseInOut(float Value);

	// Quintic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuinticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuinticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float QuinticEaseInOut(float Value);

	// Exponential Easing, Base 2

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float ExponentialEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float ExponentialEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float ExponentialEaseInOut(float Value);

	// Circular Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float CircularEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float CircularEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float CircularEaseInOut(float Value);

	// Overshooting Cubic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float BackEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float BackEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float BackEaseInOut(float Value);

	// Exponentially-Damped Sine Wave Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float ElasticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float ElasticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float ElasticEaseInOut(float Value);

	// Exponentially-Decaying Bounce Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float BounceEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float BounceEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Easing Utility", Meta = (ReturnDisplayName = "Value"))
	static float BounceEaseInOut(float Value);
};
