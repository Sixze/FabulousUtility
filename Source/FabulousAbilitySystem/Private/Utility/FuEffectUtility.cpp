#include "Utility/FuEffectUtility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEffectUtility)

const FGameplayTagContainer& UFuEffectUtility::GetOwnedTags(const TSubclassOf<UGameplayEffect> EffectClass)
{
	if (FU_ENSURE(IsValid(EffectClass)))
	{
		return EffectClass.GetDefaultObject()->InheritableOwnedTagsContainer.CombinedTags;
	}

	static const FGameplayTagContainer None;
	check(None.IsEmpty())

	return None;
}

int32 UFuEffectUtility::GetEffectStackCountByClass(const UAbilitySystemComponent* AbilitySystem,
                                                   const TSubclassOf<UGameplayEffect> EffectClass)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass)) ||
	    !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		return 0;
	}

	auto MaxCount{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		// The backing gameplay effect class must be exactly the same. This is required for
		// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

		if (ActiveEffect.Spec.Def->GetClass() == EffectClass)
		{
			MaxCount = FMath::Max(MaxCount, ActiveEffect.Spec.StackCount);
		}
	}

	return MaxCount;
}

bool UFuEffectUtility::HasActiveEffectByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectQuery.IsEmpty()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			return true;
		}
	}

	return false;
}

void UFuEffectUtility::GetActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem,
                                               const FGameplayEffectQuery& EffectQuery, TArray<FActiveGameplayEffect>& ActiveEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectQuery.IsEmpty()))
	{
		return;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			ActiveEffects.Add(ActiveEffect);
		}
	}
}

bool UFuEffectUtility::HasActiveEffectWithTag(const UAbilitySystemComponent* AbilitySystem,
                                              const FGameplayTag& EffectTag, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(EffectTag.IsValid()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			return true;
		}
	}

	return false;
}

bool UFuEffectUtility::HasActiveEffectWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
                                                  const FGameplayTagContainer& EffectTags, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectTags.IsEmpty()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags))
		{
			return true;
		}
	}

	return false;
}

int32 UFuEffectUtility::GetEffectsCountWithTag(const UAbilitySystemComponent* AbilitySystem,
                                               const FGameplayTag& EffectTag, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(EffectTag.IsValid()))
	{
		return false;
	}

	auto Count{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			Count += ActiveEffect.Spec.StackCount;
		}
	}

	return Count;
}

int32 UFuEffectUtility::GetEffectsCountWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayTagContainer& EffectTags, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectTags.IsEmpty()))
	{
		return 0;
	}

	auto Count{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(EffectTags) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(EffectTags))
		{
			Count += ActiveEffect.Spec.StackCount;
		}
	}

	return Count;
}

const FActiveGameplayEffect* UFuEffectUtility::GetActiveEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
                                                                                            const FGameplayTag& EffectTag,
                                                                                            float& TimeRemaining, float& Duration)
{
	TimeRemaining = -1.0f;
	Duration = -1.0f;

	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return nullptr;
	}

	const auto& ActiveEffects{AbilitySystem->GetActiveGameplayEffects()};

	const FActiveGameplayEffect* Result{nullptr};
	const auto Time{ActiveEffects.GetWorldTime()};

	for (auto& ActiveEffect : &ActiveEffects)
	{
		if (!ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(EffectTag) &&
		    !ActiveEffect.Spec.DynamicGrantedTags.HasTag(EffectTag))
		{
			continue;
		}

		const auto OtherDuration{ActiveEffect.GetDuration()};

		if (OtherDuration < 0.0f)
		{
			// Special case for infinite effects.

			Result = &ActiveEffect;

			TimeRemaining = FGameplayEffectConstants::INFINITE_DURATION;
			Duration = FGameplayEffectConstants::INFINITE_DURATION;
			break;
		}

		const auto OtherTimeRemaining{OtherDuration + ActiveEffect.StartWorldTime - Time};

		if (OtherTimeRemaining > TimeRemaining)
		{
			Result = &ActiveEffect;

			TimeRemaining = OtherTimeRemaining;
			Duration = OtherDuration;
		}
	}

	return Result;
}

bool UFuEffectUtility::TryGetEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
                                                                 const FGameplayTag& EffectTag, float& TimeRemaining, float& Duration)
{
	if (!IsValid(AbilitySystem))
	{
		TimeRemaining = -1.0f;
		Duration = -1.0f;
		return false;
	}

	return GetActiveEffectTimeRemainingAndDurationByTag(AbilitySystem, EffectTag, TimeRemaining, Duration) != nullptr;
}
