#include "Math/FuEasing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEasing)

float UFuEasing::SineEaseIn(const float Value)
{
	return FMath::Sin(Value * UE_HALF_PI - UE_HALF_PI) + 1.0f;
}

float UFuEasing::SineEaseOut(const float Value)
{
	return FMath::Sin(Value * UE_HALF_PI);
}

float UFuEasing::SineEaseInOut(const float Value)
{
	return (1.0f - FMath::Cos(Value * UE_PI)) * 0.5f;
}

float UFuEasing::QuadraticEaseIn(const float Value)
{
	return Value * Value;
}

float UFuEasing::QuadraticEaseOut(const float Value)
{
	return Value * (2.0f - Value);
}

float UFuEasing::QuadraticEaseInOut(const float Value)
{
	return Value < 0.5f
		       ? Value * Value * 2.0f
		       : Value * 4.0f - Value * Value * 2.0f - 1.0f;
}

float UFuEasing::CubicEaseIn(const float Value)
{
	return Value * Value * Value;
}

float UFuEasing::CubicEaseOut(const float Value)
{
	const auto X{Value - 1.0f};

	return X * X * X + 1.0f;
}

float UFuEasing::CubicEaseInOut(const float Value)
{
	if (Value < 0.5f)
	{
		return Value * Value * Value * 4.0f;
	}

	const auto X{Value * 2.0f - 2.0f};

	return X * X * X * 0.5f + 1.0f;
}

float UFuEasing::QuarticEaseIn(const float Value)
{
	return Value * Value * Value * Value;
}

float UFuEasing::QuarticEaseOut(const float Value)
{
	const auto X{Value - 1.0f};

	return (1.0f - Value) * X * X * X + 1.0f;
}

float UFuEasing::QuarticEaseInOut(const float Value)
{
	if (Value < 0.5f)
	{
		return Value * Value * Value * Value * 8.0f;
	}

	const auto X{Value - 1.0f};

	return 1.0f - X * X * X * X * 8.0f;
}

float UFuEasing::QuinticEaseIn(const float Value)
{
	return Value * Value * Value * Value * Value;
}

float UFuEasing::QuinticEaseOut(const float Value)
{
	const auto X{Value - 1.0f};

	return X * X * X * X * X + 1.0f;
}

float UFuEasing::QuinticEaseInOut(const float Value)
{
	if (Value < 0.5f)
	{
		return Value * Value * Value * Value * Value * 16.0f;
	}

	const auto X{Value * 2.0f - 2.0f};

	return X * X * X * X * X * 0.5f + 1.0f;
}

float UFuEasing::ExponentialEaseIn(const float Value)
{
	return Value > 0.0f
		       ? FMath::Pow(2.0f, Value * 10.0f - 10.0f)
		       : Value;
}

float UFuEasing::ExponentialEaseOut(const float Value)
{
	return Value < 1.0f
		       ? 1.0f - FMath::Pow(2.0f, -Value * 10.0f)
		       : Value;
}

float UFuEasing::ExponentialEaseInOut(const float Value)
{
	return Value <= 0.0f || Value >= 1.0f
		       ? Value
		       : Value < 0.5f
		       ? FMath::Pow(2.0f, Value * 20.0f - 10.0f) * 0.5f
		       : 1.0f - FMath::Pow(2.0f, 10.0f - Value * 20.0f) * 0.5f;
}

float UFuEasing::CircularEaseIn(const float Value)
{
	return 1.0f - FMath::Sqrt(1.0f - Value * Value);
}

float UFuEasing::CircularEaseOut(const float Value)
{
	return FMath::Sqrt(Value * (2.0f - Value));
}

float UFuEasing::CircularEaseInOut(const float Value)
{
	return Value < 0.5f
		       ? 0.5f - FMath::Sqrt(1.0f - Value * Value * 4.0f) * 0.5f
		       : FMath::Sqrt((3.0f - Value * 2.0f) * (Value * 2.0f - 1.0f)) * 0.5f + 0.5f;
}

float UFuEasing::BackEaseIn(const float Value)
{
	return Value * Value * Value - Value * FMath::Sin(Value * UE_PI);
}

float UFuEasing::BackEaseOut(const float Value)
{
	const auto X{1.0f - Value};

	return X * FMath::Sin(X * UE_PI) - X * X * X + 1.0f;
}

float UFuEasing::BackEaseInOut(const float Value)
{
	if (Value < 0.5f)
	{
		const auto X{Value * 2.0f};

		return (X * X * X - X * FMath::Sin(X * UE_PI)) * 0.5f;
	}

	const auto X{2.0f - Value * 2.0f};

	return (X * FMath::Sin(X * UE_PI) - X * X * X + 1.0f) * 0.5f + 0.5f;
}

float UFuEasing::ElasticEaseIn(const float Value)
{
	return FMath::Sin(Value * 13.0f * UE_HALF_PI) * FMath::Pow(2.0f, Value * 10.0f - 10.0f);
}

float UFuEasing::ElasticEaseOut(const float Value)
{
	return FMath::Sin((1.0f - Value) * 13.0f * UE_HALF_PI) * FMath::Pow(2.0f, -Value * 10.0f) + 1.0f;
}

float UFuEasing::ElasticEaseInOut(const float Value)
{
	return Value < 0.5f
		       ? FMath::Sin(Value * 26.0f * UE_HALF_PI) * FMath::Pow(2.0f, Value * 20.0f - 10.0f) * 0.5f
		       : FMath::Sin(-Value * 26.0f * UE_HALF_PI) * FMath::Pow(2.0f, 10.0f - Value * 20.0f) * 0.5f + 1.0f;
}

float UFuEasing::BounceEaseIn(const float Value)
{
	return 1.0f - BounceEaseOut(1.0f - Value);
}

float UFuEasing::BounceEaseOut(const float Value)
{
	return Value < 4.0f / 11.0f
		       ? Value * Value * 21.0f / 16.0f
		       : Value < 8.0f / 11.0f
		       ? Value * Value * 363.0f / 40.0f - Value * 99.0f / 10.0f + 17.0f / 5.0f
		       : Value < 9.0f / 10.0f
		       ? Value * Value * 4356.0f / 361.0f - Value * 35442.0f / 1805.0f + 16061.0f / 1805.0f
		       : Value * Value * 54.0f / 5.0f - Value * 513.0f / 25.0f + 268.0f / 25.0f;
}

float UFuEasing::BounceEaseInOut(const float Value)
{
	return Value < 0.5f
		       ? BounceEaseIn(Value * 2.0f) * 0.5f
		       : BounceEaseOut(Value * 2.0f - 1.0f) * 0.5f + 0.5f;
}
