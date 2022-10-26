#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_AbilityActivator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuAbilityEventDelegate);

UCLASS(DisplayName = "Fu Ability Activator Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_AbilityActivator : public UAbilityTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Ability Handle", Meta = (AllowPrivateAccess))
	FGameplayAbilitySpecHandle AbilityHandle1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient,
		DisplayName = "Cancel Ability on Destroy", Meta = (AllowPrivateAccess))
	bool bCancelAbilityOnDestroy1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityEventDelegate OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityEventDelegate OnAbilityFailed;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityEventDelegate OnAbilityEnded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Activator Ability Task")
	FFuAbilityEventDelegate OnAbilityCanceled;

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
