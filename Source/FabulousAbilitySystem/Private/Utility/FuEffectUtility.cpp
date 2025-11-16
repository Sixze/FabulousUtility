#include "Utility/FuEffectUtility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Utility/FuEffectSpecificationUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEffectUtility)

int32 UFuEffectUtility::GetEffectStackCountByClass(const UAbilitySystemComponent* AbilitySystem,
                                                   const TSubclassOf<UGameplayEffect> EffectClass)
{
	// TODO Replace StackingType with UGameplayEffect::GetStackingType() in future engine versions.
	PRAGMA_DISABLE_DEPRECATION_WARNINGS

	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass)) ||
	    !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		return 0;
	}

	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	auto MaxCount{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		// The backing gameplay effect class must be exactly the same. This is required for
		// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

		if (ActiveEffect.Spec.Def->GetClass() == EffectClass)
		{
			MaxCount = FMath::Max(MaxCount, ActiveEffect.Spec.GetStackCount());
		}
	}

	return MaxCount;
}

int32 UFuEffectUtility::GetEffectStackCountByClassSoft(const UAbilitySystemComponent* AbilitySystem,
                                                       const TSoftClassPtr<UGameplayEffect>& EffectClassSoft)
{
	// If the effect is not loaded, then there are no active effects.

	const TSubclassOf<UGameplayEffect> EffectClass{EffectClassSoft.Get()};
	if (!FU_ENSURE(!EffectClassSoft.IsNull()) || !IsValid(EffectClass))
	{
		return 0;
	}

	return GetEffectStackCountByClass(AbilitySystem, EffectClass);
}

bool UFuEffectUtility::HasActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery)
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

void UFuEffectUtility::FindActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem,
                                                const FGameplayEffectQuery& EffectQuery, TArray<FActiveGameplayEffect>& ActiveEffects)
{
	ActiveEffects.Reset();

	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectQuery.IsEmpty()))
	{
		return;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			ActiveEffects.Emplace(ActiveEffect);
		}
	}
}

bool UFuEffectUtility::HasActiveEffectsWithTag(const UAbilitySystemComponent* AbilitySystem,
                                               const FGameplayTag& Tag, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if ((!ActiveEffect.bIsInhibited || bIgnoreInhibitedEffects) &&
		    UFuEffectSpecificationUtility::HasGrantedTag(ActiveEffect.Spec, Tag))
		{
			return true;
		}
	}

	return false;
}

bool UFuEffectUtility::HasActiveEffectsWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayTagContainer& Tags, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!Tags.IsEmpty()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if ((!ActiveEffect.bIsInhibited || bIgnoreInhibitedEffects) &&
		    UFuEffectSpecificationUtility::HasAnyGrantedTags(ActiveEffect.Spec, Tags))
		{
			return true;
		}
	}

	return false;
}

int32 UFuEffectUtility::GetEffectsCountByTag(const UAbilitySystemComponent* AbilitySystem,
                                             const FGameplayTag& Tag, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	auto EffectsCount{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (UFuEffectSpecificationUtility::HasGrantedTag(ActiveEffect.Spec, Tag))
		{
			EffectsCount += ActiveEffect.Spec.GetStackCount();
		}
	}

	return EffectsCount;
}

int32 UFuEffectUtility::GetEffectsCountWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayTagContainer& Tags, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!Tags.IsEmpty()))
	{
		return 0;
	}

	auto EffectsCount{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if ((!ActiveEffect.bIsInhibited || bIgnoreInhibitedEffects) &&
		    UFuEffectSpecificationUtility::HasAnyGrantedTags(ActiveEffect.Spec, Tags))
		{
			EffectsCount += ActiveEffect.Spec.GetStackCount();
		}
	}

	return EffectsCount;
}

const FActiveGameplayEffect* UFuEffectUtility::GetActiveEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
                                                                                            const FGameplayTag& Tag,
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
		if (!UFuEffectSpecificationUtility::HasGrantedTag(ActiveEffect.Spec, Tag))
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
                                                                 const FGameplayTag& Tag, float& TimeRemaining, float& Duration)
{
	if (!IsValid(AbilitySystem))
	{
		TimeRemaining = -1.0f;
		Duration = -1.0f;
		return false;
	}

	return GetActiveEffectTimeRemainingAndDurationByTag(AbilitySystem, Tag, TimeRemaining, Duration) != nullptr;
}
