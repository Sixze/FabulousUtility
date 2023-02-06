#include "Animation/FuAnimationUtility.h"

#include "FuMacros.h"
#include "Animation/AnimSequenceBase.h"

float UFuAnimationUtility::GetScaledPlayLength(const UAnimSequenceBase* Sequence)
{
	if (!FU_ENSURE(IsValid(Sequence)))
	{
		return 0.0f;
	}

	const auto PlayRate{FMath::Abs(Sequence->RateScale)};
	return PlayRate > SMALL_NUMBER ? Sequence->GetPlayLength() / PlayRate : 0.0f;
}
