#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_AbilityActivator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuAbilityActivatorDelegate);

UCLASS(DisplayName = "Fu Ability Activator Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_AbilityActivator : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayAbilitySpecHandle AbilityHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bCancelAbilityOnDestroy : 1 {false};

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Ability Activator")
	FFuAbilityActivatorDelegate OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Ability Activator")
	FFuAbilityActivatorDelegate OnAbilityFailed;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Ability Activator")
	FFuAbilityActivatorDelegate OnAbilityEnded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Ability Activator")
	FFuAbilityActivatorDelegate OnAbilityCanceled;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_AbilityActivator* ActivateAbilityByHandle(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Ability Handle") FGameplayAbilitySpecHandle InAbilityHandle,
		UPARAM(DisplayName = "Cancel Ability on Destroy") bool bInCancelAbilityOnDestroy);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData);
};
