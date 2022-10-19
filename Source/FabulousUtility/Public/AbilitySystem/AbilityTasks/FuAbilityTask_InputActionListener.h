#pragma once

#include "EnhancedInputComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_InputActionListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuInputDelegate, const UInputAction*, InputAction, FInputActionValue, Value);

UCLASS(DisplayName = "Fu Input Action Listener Ability Task")
class FABULOUSUTILITY_API UFuAbilityTask_InputActionListener : public UAbilityTask
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UEnhancedInputComponent> Input;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Input Actions", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UInputAction>> InputActions1;

	TArray<FInputBindingHandle> InputBindings;

public:
	// Triggering occurred after one or more processing ticks.
	// ETriggerState (None -> Triggered, Ongoing -> Triggered, Triggered -> Triggered).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Input Action Listener Ability Async")
	FFuInputDelegate OnTriggered;

	// An event has occurred that has begun Trigger evaluation. Triggered may also occur this frame.
	// ETriggerState (None -> Ongoing, None -> Triggered).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Input Action Listener Ability Async")
	FFuInputDelegate OnStarted;

	// Triggering is still being processed.
	// ETriggerState (Ongoing -> Ongoing).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu InputAction Listener Ability Async")
	FFuInputDelegate OnOngoing;

	// Triggering has been canceled.
	// ETriggerState (Ongoing -> None).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Input Action Listener Ability Async")
	FFuInputDelegate OnCanceled;

	// The trigger state has transitioned from Triggered to None this frame, i.e. Triggering has finished.
	// ETriggerState (Triggered -> None).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Input Action Listener Ability Async")
	FFuInputDelegate OnCompleted;

public:
	// Warning! This task only works locally, its events will never be called on the server!
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_InputActionListener* FuWaitForInputAction(UGameplayAbility* OwningAbility, UInputAction* InputAction);

	// Warning! This task only works locally, its events will never be called on the server!
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability", BlueprintInternalUseOnly,
		Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_InputActionListener* FuWaitForInputActions(UGameplayAbility* OwningAbility,
	                                                                 const TArray<UInputAction*>& InputActions);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void BindActions();

	void UnBindActions();

	UFUNCTION()
	void OnPawnRestarted(APawn* Pawn);

	void Input_OnTriggered(const FInputActionInstance& ActionInstance);

	void Input_OnStarted(const FInputActionInstance& ActionInstance);

	void Input_OnOngoing(const FInputActionInstance& ActionInstance);

	void Input_OnCanceled(const FInputActionInstance& ActionInstance);

	void Input_OnCompleted(const FInputActionInstance& ActionInstance);
};
