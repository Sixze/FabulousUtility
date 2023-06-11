#include "Utility/FuAttributeUtility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAttributeUtility)

bool UFuAttributeUtility::TryGetAttributeValue(const UAbilitySystemComponent* AbilitySystem,
                                               const FGameplayAttribute& Attribute, float& Value)
{
	if (!IsValid(AbilitySystem))
	{
		Value = 0.0f;
		return false;
	}

	auto bFound{false};
	Value = AbilitySystem->GetGameplayAttributeValue(Attribute, bFound);

	return bFound;
}

bool UFuAttributeUtility::TryGetAttributeValueOnAbility(const UGameplayAbility* Ability, const FGameplayAttribute& Attribute, float& Value)
{
	if (!IsValid(Ability))
	{
		Value = 0.0f;
		return false;
	}

	return TryGetAttributeValue(Ability->GetAbilitySystemComponentFromActorInfo(), Attribute, Value);
}

bool UFuAttributeUtility::TryGetAttributePercent(const UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute& Attribute,
                                                 const FGameplayAttribute& MaxAttribute, float& Percent)
{
	float Value, MaxValue;

	if (!TryGetAttributeValue(AbilitySystem, Attribute, Value) ||
	    !TryGetAttributeValue(AbilitySystem, MaxAttribute, MaxValue) ||
	    FMath::IsNearlyZero(MaxValue))
	{
		Percent = 0.0f;
		return false;
	}

	Percent = Value / MaxValue;
	return true;
}

bool UFuAttributeUtility::TryGetAttributePercentOnAbility(const UGameplayAbility* Ability, const FGameplayAttribute& Attribute,
                                                          const FGameplayAttribute& MaxAttribute, float& Percent)
{
	if (!IsValid(Ability))
	{
		Percent = 0.0f;
		return false;
	}

	return TryGetAttributePercent(Ability->GetAbilitySystemComponentFromActorInfo(), Attribute, MaxAttribute, Percent);
}

void UFuAttributeUtility::AdjustAttributeForMaxChange(UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute& Attribute,
                                                      const float PreviousMaxValue, const float NewMaxValue)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || FMath::IsNearlyEqual(PreviousMaxValue, NewMaxValue))
	{
		return;
	}

	const auto NewValue{
		PreviousMaxValue > UE_SMALL_NUMBER
			? AbilitySystem->GetNumericAttributeBase(Attribute) * NewMaxValue / PreviousMaxValue
			: NewMaxValue
	};

	AbilitySystem->SetNumericAttributeBase(Attribute, NewValue);
}
