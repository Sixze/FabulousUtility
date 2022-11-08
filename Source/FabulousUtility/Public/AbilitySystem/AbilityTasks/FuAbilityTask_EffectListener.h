#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_EffectListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuEffectListenerDelegate, FActiveGameplayEffectHandle, EffectHandle);

UCLASS(DisplayName = "Fu Effect Listener Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_EffectListener : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Effect Tags")
	FGameplayTagContainer EffectTags1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Listener Ability Task")
	FFuEffectListenerDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Listener Ability Task")
	FFuEffectListenerDelegate OnEffectRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_EffectListener* FuWaitForEffectChangeByTag(UGameplayAbility* OwningAbility, FGameplayTag EffectTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_EffectListener* FuWaitForEffectChangeByTags(UGameplayAbility* OwningAbility, FGameplayTagContainer EffectTags);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem,
	                                 const FGameplayEffectSpec& EffectSpecification,
	                                 FActiveGameplayEffectHandle EffectHandle) const;

	void OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;
};
