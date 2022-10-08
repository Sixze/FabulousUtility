#include "Animation/FuAnimationUtility.h"

#include "Animation/AnimSequenceBase.h"

float UFuAnimationUtility::GetSequenceScaledPlayLength(const UAnimSequenceBase* Sequence)
{
	const auto PlayRate{FMath::Abs(Sequence->RateScale)};

	return IsValid(Sequence) && PlayRate > SMALL_NUMBER
		       ? Sequence->GetPlayLength() / PlayRate
		       : 0.0f;
}
