#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_ActivateAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuAbilityEventDelegate);

UCLASS(DisplayName = "Fu Activate Ability Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_ActivateAbility : public UAbilityTask
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
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Activate Ability Ability Task")
	FFuAbilityEventDelegate OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Activate Ability Ability Task")
	FFuAbilityEventDelegate OnAbilityFailed;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Activate Ability Ability Task")
	FFuAbilityEventDelegate OnAbilityEnded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Activate Ability Ability Task")
	FFuAbilityEventDelegate OnAbilityCanceled;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_ActivateAbility* FuActivateAbilityByHandle(UGameplayAbility* OwningAbility,
	                                                                 FGameplayAbilitySpecHandle AbilityHandle,
	                                                                 bool bCancelAbilityOnDestroy);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData);
};
