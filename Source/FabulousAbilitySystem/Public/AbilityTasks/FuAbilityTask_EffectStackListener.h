#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_EffectStackListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFuEffectStackListenerDelegate, FActiveGameplayEffectHandle, EffectHandle,
                                               int32, NewCount, int32, PreviousCount);

UCLASS(DisplayName = "Fu Effect Stack Listener Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_EffectStackListener : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TSubclassOf<UGameplayEffect> EffectClass;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Effect Stack Listener")
	FFuEffectStackListenerDelegate OnStackChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_EffectStackListener* WaitForEffectStackChange(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Effect Class") TSubclassOf<UGameplayEffect> InEffectClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_EffectStackListener* WaitForEffectStackChangeSoft(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Effect Class") TSoftClassPtr<UGameplayEffect> InEffectClass);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void AbilitySystem_OnEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewCount, int32 PreviousCount) const;
};
