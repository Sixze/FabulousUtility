#include "Utility/FuEffectSpecificationUtility.h"

#include "FuMacros.h"
#include "GameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEffectSpecificationUtility)

bool UFuEffectSpecificationUtility::HasAssetTag(const FGameplayEffectSpec& EffectSpecification, const FGameplayTag& Tag)
{
	return EffectSpecification.Def->GetAssetTags().HasTag(Tag) ||
	       EffectSpecification.GetDynamicAssetTags().HasTag(Tag);
}

bool UFuEffectSpecificationUtility::HasAnyAssetTags(const FGameplayEffectSpec& EffectSpecification, const FGameplayTagContainer& Tags)
{
	return EffectSpecification.Def->GetAssetTags().HasAny(Tags) ||
	       EffectSpecification.GetDynamicAssetTags().HasAny(Tags);
}

bool UFuEffectSpecificationUtility::HasGrantedTag(const FGameplayEffectSpec& EffectSpecification, const FGameplayTag& Tag)
{
	return EffectSpecification.Def->GetGrantedTags().HasTag(Tag) ||
	       EffectSpecification.DynamicGrantedTags.HasTag(Tag);
}

bool UFuEffectSpecificationUtility::HasAnyGrantedTags(const FGameplayEffectSpec& EffectSpecification, const FGameplayTagContainer& Tags)
{
	return EffectSpecification.Def->GetGrantedTags().HasAny(Tags) ||
	       EffectSpecification.DynamicGrantedTags.HasAny(Tags);
}

float UFuEffectSpecificationUtility::FindSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectHandle,
                                                              const FGameplayTag& SetByCallerTag)
{
	const auto* EffectSpecification{EffectHandle.Data.Get()};

	return FU_ENSURE(EffectSpecification != nullptr)
		       ? EffectSpecification->GetSetByCallerMagnitude(SetByCallerTag, false)
		       : 0.0f;
}
