#include "AbilitySystem/AbilityTasks/FuAbilityTask_InputActionListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

UFuAbilityTask_InputActionListener* UFuAbilityTask_InputActionListener::FuWaitForInputAction(
	UGameplayAbility* OwningAbility, UInputAction* InputAction)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	Task->Input = Cast<UEnhancedInputComponent>(Task->AbilitySystemComponent->GetAvatarActor_Direct()->InputComponent);

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

	Task->Input = Cast<UEnhancedInputComponent>(Task->AbilitySystemComponent->GetAvatarActor_Direct()->InputComponent);

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

	if (InputActions1.IsEmpty())
	{
		EndTask();
		return;
	}

	auto* Pawn{Cast<APawn>(AbilitySystemComponent->GetAvatarActor_Direct())};
	if (IsValid(Pawn))
	{
		Pawn->ReceiveRestartedDelegate.AddDynamic(this, &ThisClass::OnPawnRestarted);
	}

	BindActions();
}

void UFuAbilityTask_InputActionListener::OnDestroy(const bool bInOwnerFinished)
{
	auto* Pawn{Cast<APawn>(AbilitySystemComponent->GetAvatarActor_Direct())};
	if (IsValid(Pawn))
	{
		Pawn->ReceiveRestartedDelegate.RemoveAll(this);
	}

	UnBindActions();

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_InputActionListener::BindActions()
{
	if (Input.IsValid())
	{
		for (const auto InputAction : InputActions1)
		{
			InputBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnTriggered));
			InputBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Input_OnStarted));
			InputBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Ongoing, this, &ThisClass::Input_OnOngoing));
			InputBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnCanceled));
			InputBindings.Add(Input->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnCompleted));
		}
	}
}

void UFuAbilityTask_InputActionListener::UnBindActions()
{
	if (Input.IsValid())
	{
		for (const auto& InputBinding : InputBindings)
		{
			Input->RemoveBinding(InputBinding);
		}

		InputBindings.Reset();
	}
}

void UFuAbilityTask_InputActionListener::OnPawnRestarted(APawn* Pawn)
{
	UnBindActions();

	Input = Cast<UEnhancedInputComponent>(Pawn->InputComponent);

	BindActions();
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
