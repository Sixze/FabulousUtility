#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEasing.generated.h"

// https://github.com/warrenm/AHEasing

UCLASS()
class FABULOUSUTILITY_API UFuEasing : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Sine Wave Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float SineEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float SineEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float SineEaseInOut(float Value);

	// Quadratic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuadraticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuadraticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuadraticEaseInOut(float Value);

	// Cubic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float CubicEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float CubicEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float CubicEaseInOut(float Value);

	// Quartic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuarticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuarticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuarticEaseInOut(float Value);

	// Quintic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuinticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuinticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float QuinticEaseInOut(float Value);

	// Exponential Easing, Base 2

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float ExponentialEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float ExponentialEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float ExponentialEaseInOut(float Value);

	// Circular Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float CircularEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float CircularEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float CircularEaseInOut(float Value);

	// Overshooting Cubic Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float BackEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float BackEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float BackEaseInOut(float Value);

	// Exponentially-Damped Sine Wave Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float ElasticEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float ElasticEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float ElasticEaseInOut(float Value);

	// Exponentially-Decaying Bounce Easing

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float BounceEaseIn(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float BounceEaseOut(float Value);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Easing")
	static float BounceEaseInOut(float Value);
};
