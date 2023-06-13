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
	bool bCancelAbilityOnDestroy;

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
	static UFuAbilityTask_AbilityActivator* FuActivateAbilityByHandle(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Ability Handle") FGameplayAbilitySpecHandle InAbilityHandle,
		UPARAM(DisplayName = "Cancel Ability on Destroy") bool bInCancelAbilityOnDestroy);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnAbilityEnded(const FAbilityEndedData& EndedData);
};
