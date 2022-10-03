#include "AbilitySystem/AbilityTasks/FuAbilityTask_InputActionListener.h"

#include "AbilitySystemComponent.h"
#include "EnhancedPlayerInput.h"
#include "FuMacros.h"
#include "GameFramework/PlayerController.h"

UFuAbilityTask_InputActionListener* UFuAbilityTask_InputActionListener::FuWaitForInputAction(
	UGameplayAbility* OwningAbility, UInputAction* InputAction)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	// Listen for input on the player controller instead of the avatar.

	const auto* PlayerController{Task->AbilitySystemComponent->AbilityActorInfo->PlayerController.Get()};
	Task->Input = IsValid(PlayerController) ? Cast<UEnhancedInputComponent>(PlayerController->InputComponent) : nullptr;

	if (FU_ENSURE(IsValid(InputAction)))
	{
		Task->InputActions1.AddUnique(InputAction);
	}

	return Task;
}

UFuAbilityTask_InputActionListener* UFuAbilityTask_InputActionListener::FuWaitForInputActions(
	UGameplayAbility* OwningAbility, const TArray<UInputAction*>& InputActions)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	// Listen for input on the player controller instead of the avatar.

	const auto* PlayerController{Task->AbilitySystemComponent->AbilityActorInfo->PlayerController.Get()};
	Task->Input = IsValid(PlayerController) ? Cast<UEnhancedInputComponent>(PlayerController->InputComponent) : nullptr;

	for (const auto& InputAction : InputActions)
	{
		if (FU_ENSURE(IsValid(InputAction)))
		{
			Task->InputActions1.AddUnique(InputAction);
		}
	}

	return Task;
}

void UFuAbilityTask_InputActionListener::Activate()
{
	Super::Activate();

	if (!Input.IsValid() || InputActions1.IsEmpty())
	{
		EndTask();
		return;
	}

	for (const auto InputAction : InputActions1)
	{
		ActionBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnTriggered));
		ActionBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Input_OnStarted));
		ActionBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Ongoing, this, &ThisClass::Input_OnOngoing));
		ActionBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnCanceled));
		ActionBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnCompleted));
	}

	const auto* PlayerInput{Cast<UEnhancedPlayerInput>(AbilitySystemComponent->AbilityActorInfo->PlayerController->PlayerInput)};

	if (!IsValid(PlayerInput) || !ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	// UInputComponent::bBlockInput is ignored when broadcasting events on task activation!

	for (const auto InputAction : InputActions1)
	{
		const auto* ActionInstance{PlayerInput->FindActionInstanceData(InputAction)};
		if (ActionInstance == nullptr)
		{
			continue;
		}

		// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
		// ReSharper disable once CppIncompleteSwitchStatement
		switch (ActionInstance->GetTriggerEvent())
		{
			case ETriggerEvent::Triggered:
				OnTriggered.Broadcast(ActionInstance->GetSourceAction(), ActionInstance->GetValue());
				break;

			case ETriggerEvent::Started:
				OnStarted.Broadcast(ActionInstance->GetSourceAction(), ActionInstance->GetValue());
				break;

			case ETriggerEvent::Ongoing:
				// There is no way to tell which event was before ETriggerEvent::Ongoing, and because of that, we cannot broadcast
				// ETriggerEvent::Triggered and ETriggerEvent::Started before ETriggerEvent::Ongoing during task activation.
				OnOngoing.Broadcast(ActionInstance->GetSourceAction(), ActionInstance->GetValue());
				break;

			case ETriggerEvent::Canceled:
				OnCanceled.Broadcast(ActionInstance->GetSourceAction(), ActionInstance->GetValue());
				break;

			case ETriggerEvent::Completed:
				OnCompleted.Broadcast(ActionInstance->GetSourceAction(), ActionInstance->GetValue());
				break;
		}
	}
}

void UFuAbilityTask_InputActionListener::OnDestroy(const bool bInOwnerFinished)
{
	if (Input.IsValid())
	{
		for (const auto& ActionBinding : ActionBindings)
		{
			Input->RemoveBinding(ActionBinding);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnTriggered(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTriggered.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnStarted(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnStarted.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnOngoing(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnOngoing.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnCanceled(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCanceled.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnCompleted(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCompleted.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}
