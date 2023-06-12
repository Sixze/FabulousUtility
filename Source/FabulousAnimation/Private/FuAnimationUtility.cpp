#include "FuAnimationUtility.h"

#include "FuMacros.h"
#include "Animation/AnimSequenceBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnimationUtility)

float UFuAnimationUtility::GetScaledPlayLength(const UAnimSequenceBase* Sequence)
{
	if (!FU_ENSURE(IsValid(Sequence)))
	{
		return 0.0f;
	}

	const auto PlayRate{FMath::Abs(Sequence->RateScale)};
	return PlayRate > UE_SMALL_NUMBER ? Sequence->GetPlayLength() / PlayRate : 0.0f;
}
