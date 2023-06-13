#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_InputListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuInputListenerDelegate);

UCLASS(DisplayName = "Fu Input Listener Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_InputListener : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bCheckInitialInputState;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Input Listener Ability Task")
	FFuInputListenerDelegate OnInputPressed;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Input Listener Ability Task")
	FFuInputListenerDelegate OnInputReleased;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Tasks", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_InputListener* FuWaitForInput(UGameplayAbility* OwningAbility,
	                                                    UPARAM(DisplayName = "Check Initial Input State") bool bInCheckInitialInputState);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void AbilitySystem_OnInputPressed();

	void AbilitySystem_OnInputReleased();
};
