#include "Utility/FuEffectSpecificationUtility.h"

#include "FuMacros.h"
#include "GameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEffectSpecificationUtility)

float UFuEffectSpecificationUtility::GetSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectHandle,
                                                             const FGameplayTag& SetByCallerTag)
{
	const auto* EffectSpecification{EffectHandle.Data.Get()};

	return FU_ENSURE(EffectSpecification != nullptr)
		       ? EffectSpecification->GetSetByCallerMagnitude(SetByCallerTag, false)
		       : 0.0f;
}
