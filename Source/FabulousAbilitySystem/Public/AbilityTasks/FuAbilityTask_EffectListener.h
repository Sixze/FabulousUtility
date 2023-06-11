#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_EffectListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuEffectListenerDelegate, FActiveGameplayEffectHandle, EffectHandle);

UCLASS(DisplayName = "Fu Effect Listener Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_EffectListener : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer EffectTags;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Listener Ability Task")
	FFuEffectListenerDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Listener Ability Task")
	FFuEffectListenerDelegate OnEffectRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_EffectListener* FuWaitForEffectChangeByTag(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Effect Tag") FGameplayTag InEffectTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_EffectListener* FuWaitForEffectChangeByTags(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Effect Tags") FGameplayTagContainer InEffectTags);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;
};
