#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_AttributeListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFuAttributeListenerDelegate, const FGameplayAttribute&, Attribute,
                                               float, NewValue, float, PreviousValue);

UCLASS(DisplayName = "Fu Attribute Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_AttributeListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Attributes")
	TArray<FGameplayAttribute> Attributes1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Skip Equal Values on Server")
	bool bSkipEqualValuesOnServer1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Attribute Listener Ability Async")
	FFuAttributeListenerDelegate OnAttributeChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* FuListenForAttributeChangeActor(const AActor* Actor,
	                                                                          FGameplayAttribute Attribute,
	                                                                          bool bSkipEqualValuesOnServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* FuListenForAttributesChangeActor(const AActor* Actor,
	                                                                           const TArray<FGameplayAttribute>& Attributes,
	                                                                           bool bSkipEqualValuesOnServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* FuListenForAttributeChange(UAbilitySystemComponent* AbilitySystem,
	                                                                     FGameplayAttribute Attribute,
	                                                                     bool bSkipEqualValuesOnServer = true);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AttributeListener* FuListenForAttributesChange(UAbilitySystemComponent* AbilitySystem,
	                                                                      const TArray<FGameplayAttribute>& Attributes,
	                                                                      bool bSkipEqualValuesOnServer = true);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnAttributeChanged(const FOnAttributeChangeData& Data) const;
};
