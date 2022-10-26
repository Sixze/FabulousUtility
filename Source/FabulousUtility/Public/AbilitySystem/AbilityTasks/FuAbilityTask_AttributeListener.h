#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/AbilityAsync/FuAbilityAsync_AttributeListener.h"
#include "FuAbilityTask_AttributeListener.generated.h"

UCLASS(DisplayName = "Fu Attribute Listener Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_AttributeListener : public UAbilityTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Attributes", Meta = (AllowPrivateAccess))
	TArray<FGameplayAttribute> Attributes1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Attribute Listener Ability Task")
	FFuAttributeChangedDelegate OnAttributeChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_AttributeListener* FuWaitForAttributeChange(UGameplayAbility* OwningAbility, FGameplayAttribute Attribute);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_AttributeListener* FuWaitForAttributesChange(UGameplayAbility* OwningAbility,
	                                                                   const TArray<FGameplayAttribute>& Attributes);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnAttributeChanged(const FOnAttributeChangeData& Data) const;
};
