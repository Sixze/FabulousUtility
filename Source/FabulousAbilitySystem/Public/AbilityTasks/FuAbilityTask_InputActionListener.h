#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_InputActionListener.generated.h"

struct FInputBindingHandle;
struct FInputActionInstance;
class UEnhancedInputComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuInputActionListenerDelegate, const UInputAction*, InputAction,
                                             const FInputActionValue&, Value);

UCLASS(DisplayName = "Fu Input Action Listener Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_InputActionListener : public UAbilityTask
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UEnhancedInputComponent> Input;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TArray<TObjectPtr<UInputAction>> InputActions;

	TArray<FInputBindingHandle> InputBindings;

public:
	// Triggering occurred after one or more processing ticks.
	// ETriggerState (None -> Triggered, Ongoing -> Triggered, Triggered -> Triggered).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Input Action Listener")
	FFuInputActionListenerDelegate OnActionTriggered;

	// An event has occurred that has begun Trigger evaluation. Triggered may also occur this frame.
	// ETriggerState (None -> Ongoing, None -> Triggered).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Input Action Listener")
	FFuInputActionListenerDelegate OnActionStarted;

	// Triggering is still being processed.
	// ETriggerState (Ongoing -> Ongoing).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|InputAction Listener")
	FFuInputActionListenerDelegate OnActionOngoing;

	// Triggering has been canceled.
	// ETriggerState (Ongoing -> None).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Input Action Listener")
	FFuInputActionListenerDelegate OnActionCanceled;

	// The trigger state has transitioned from Triggered to None this frame, i.e. Triggering has finished.
	// ETriggerState (Triggered -> None).
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Input Action Listener")
	FFuInputActionListenerDelegate OnActionCompleted;

public:
	// Warning! This task only works locally, its events will never be called on the server!
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_InputActionListener* WaitForInputAction(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Input Action") UInputAction* InInputAction);

	// Warning! This task only works locally, its events will never be called on the server!
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_InputActionListener* WaitForInputActions(
		UGameplayAbility* OwningAbility,
		UPARAM(DisplayName = "Input Actions") const TArray<UInputAction*>& InInputActions);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void BindActions();

	void UnBindActions();

	UFUNCTION()
	void Pawn_OnRestarted(APawn* Pawn);

	void Input_OnActionTriggered(const FInputActionInstance& ActionInstance);

	void Input_OnActionStarted(const FInputActionInstance& ActionInstance);

	void Input_OnActionOngoing(const FInputActionInstance& ActionInstance);

	void Input_OnActionCanceled(const FInputActionInstance& ActionInstance);

	void Input_OnActionCompleted(const FInputActionInstance& ActionInstance);
};
