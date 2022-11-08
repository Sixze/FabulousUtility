#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_AbilityActivator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuAbilityActivatorDelegate);

UCLASS(DisplayName = "Fu Ability Activator Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_AbilityActivator : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Ability Handle")
	FGameplayAbilitySpecHandle AbilityHandle1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Cancel Ability on Destroy")
	bool bCancelAbilityOnDestroy1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityActivatorDelegate OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityActivatorDelegate OnAbilityFailed;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityActivatorDelegate OnAbilityEnded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityActivatorDelegate OnAbilityCanceled;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_AbilityActivator* FuActivateAbilityByHandle(UGameplayAbility* OwningAbility,
	                                                                  FGameplayAbilitySpecHandle AbilityHandle,
	                                                                  bool bCancelAbilityOnDestroy);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData);
};
