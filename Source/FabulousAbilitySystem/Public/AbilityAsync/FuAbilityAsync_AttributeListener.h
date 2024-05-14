#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_AttributeListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFuAttributeListenerDelegate, const FGameplayAttribute&, Attribute,
                                               float, NewValue, float, PreviousValue);

UCLASS(DisplayName = "Fu Attribute Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_AttributeListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TArray<FGameplayAttribute> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bSkipEqualValuesOnServer : 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Attribute Listener")
	FFuAttributeListenerDelegate OnAttributeChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* ListenForAttributeChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Attribute") FGameplayAttribute InAttribute,
		UPARAM(DisplayName = "Skip Equal Values on Server") bool bInSkipEqualValuesOnServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* ListenForAttributesChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Attributes") const TArray<FGameplayAttribute>& InAttributes,
		UPARAM(DisplayName = "Skip Equal Values on Server") bool bInSkipEqualValuesOnServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* ListenForAttributeChange(
		UAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Attribute") FGameplayAttribute InAttribute,
		UPARAM(DisplayName = "Skip Equal Values on Server") bool bInSkipEqualValuesOnServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* ListenForAttributesChange(
		UAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Attributes") const TArray<FGameplayAttribute>& InAttributes,
		UPARAM(DisplayName = "Skip Equal Values on Server") bool bInSkipEqualValuesOnServer = true);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnAttributeChanged(const FOnAttributeChangeData& ChangeData) const;
};
