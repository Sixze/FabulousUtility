#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#define FU_ATTRIBUTE_ACCESSORS(AttributeName) \
	static const FGameplayAttribute& Get##AttributeName##Attribute() \
	{ \
		static const FGameplayAttribute Attribute{FindFieldChecked<FProperty>(StaticClass(), GET_MEMBER_NAME_CHECKED(ThisClass, AttributeName))}; \
		return Attribute; \
	} \
	\
	float Get##AttributeName() const \
	{ \
		return AttributeName.GetCurrentValue(); \
	} \
	\
	float Get##AttributeName##Base() const \
	{ \
		return AttributeName.GetBaseValue(); \
	} \
	\
	void Set##AttributeName##Base(const float New##AttributeName) \
	{ \
		auto* AbilitySystem{GetOwningAbilitySystemComponent()}; \
		if (FU_ENSURE(IsValid(AbilitySystem))) \
		{ \
			AbilitySystem->SetNumericAttributeBase(Get##AttributeName##Attribute(), New##AttributeName); \
		}; \
	} \
	\
	void Initialize##AttributeName(const float Initial##AttributeName) \
	{ \
		AttributeName.SetBaseValue(Initial##AttributeName); \
		AttributeName.SetCurrentValue(Initial##AttributeName); \
	}

#define FU_ATTRIBUTE_ON_REPLICATED(AttributeName, PreviousValue) \
	GetOwningAbilitySystemComponentChecked()->SetBaseAttributeValueFromReplication(Get##AttributeName##Attribute(), AttributeName, PreviousValue);

#define FU_ATTRIBUTE_CAPTURE_DEFINITION(AttributeSet, AttributeName, AttributeCaptureSource, bSnapshot) \
	static const FGameplayEffectAttributeCaptureDefinition& AttributeName##Definition() \
	{ \
		static const FGameplayEffectAttributeCaptureDefinition Definition{AttributeSet::Get##AttributeName##Attribute(), EGameplayEffectAttributeCaptureSource::AttributeCaptureSource, bSnapshot}; \
		return Definition; \
	}
