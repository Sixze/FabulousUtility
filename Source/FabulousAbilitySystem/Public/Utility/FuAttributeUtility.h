#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAttributeUtility.generated.h"

class UGameplayAbility;

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAttributeUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Attribute Utility",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Attribute"))
	static bool TryGetAttributeValue(const UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute& Attribute, float& Value);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Attribute Utility",
		Meta = (DefaultToSelf = "Ability", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Attribute"))
	static bool TryGetAttributeValueFromAbility(const UGameplayAbility* Ability, const FGameplayAttribute& Attribute, float& Value);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Attribute Utility",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Attribute, MaxAttribute"))
	static bool TryGetAttributePercent(const UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute& Attribute,
	                                   const FGameplayAttribute& MaxAttribute, float& Percent);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Attribute Utility",
		Meta = (DefaultToSelf = "Ability", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Attribute, MaxAttribute"))
	static bool TryGetAttributePercentFromAbility(const UGameplayAbility* Ability, const FGameplayAttribute& Attribute,
	                                              const FGameplayAttribute& MaxAttribute, float& Percent);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Attribute Utility",
		Meta = (AutoCreateRefTerm = "Attribute, AttributeData, MaxAttributeData"))
	static void AdjustAttributeForMaxChange(UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute& Attribute,
	                                        float PreviousMaxValue, float NewMaxValue);
};
