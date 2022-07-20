#include "AbilitySystem/Utility/FuAttributeUtility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

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

void UFuAttributeUtility::AdjustAttributeForMaxChange(UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute& Attribute,
                                                      const FGameplayAttributeData& AttributeData,
                                                      const FGameplayAttributeData& MaxAttributeData, const float NewMaxValue)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilitySystem->HasAttributeSetForAttribute(Attribute)))
	{
		return;
	}

	const auto PreviousMaxValue{MaxAttributeData.GetCurrentValue()};
	if (FMath::IsNearlyEqual(PreviousMaxValue, NewMaxValue))
	{
		return;
	}

	const auto Value{AttributeData.GetCurrentValue()};

	const auto NewDelta{
		PreviousMaxValue > SMALL_NUMBER
			? Value * NewMaxValue / PreviousMaxValue - Value
			: NewMaxValue
	};

	AbilitySystem->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Additive, NewDelta);
}
