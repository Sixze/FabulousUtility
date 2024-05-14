#include "AbilityTasks/FuAbilityTask_InputActionListener.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "FuMacros.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_InputActionListener)

UFuAbilityTask_InputActionListener* UFuAbilityTask_InputActionListener::WaitForInputAction(UGameplayAbility* OwningAbility,
                                                                                           UInputAction* InInputAction)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};
	Task->Input = Cast<UEnhancedInputComponent>(Task->AbilitySystemComponent->GetAvatarActor_Direct()->InputComponent);

	if (FU_ENSURE(IsValid(InInputAction)))
	{
		Task->InputActions.AddUnique(InInputAction);
	}

	return Task;
}

UFuAbilityTask_InputActionListener* UFuAbilityTask_InputActionListener::WaitForInputActions(UGameplayAbility* OwningAbility,
                                                                                            const TArray<UInputAction*>& InInputActions)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};
	Task->Input = Cast<UEnhancedInputComponent>(Task->AbilitySystemComponent->GetAvatarActor_Direct()->InputComponent);
	Task->InputActions.Reserve(InInputActions.Num());

	for (const auto& InputAction : InInputActions)
	{
		if (FU_ENSURE(IsValid(InputAction)))
		{
			Task->InputActions.AddUnique(InputAction);
		}
	}

	return Task;
}

void UFuAbilityTask_InputActionListener::Activate()
{
	Super::Activate();

	if (InputActions.IsEmpty())
	{
		EndTask();
		return;
	}

	auto* Pawn{Cast<APawn>(AbilitySystemComponent->GetAvatarActor_Direct())};
	if (IsValid(Pawn))
	{
		Pawn->ReceiveRestartedDelegate.AddUniqueDynamic(this, &ThisClass::Pawn_OnRestarted);
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
		InputBindings.Reserve(InputActions.Num() * 5);

		for (const auto InputAction : InputActions)
		{
			InputBindings.Emplace(Input->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnActionTriggered));
			InputBindings.Emplace(Input->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Input_OnActionStarted));
			InputBindings.Emplace(Input->BindAction(InputAction, ETriggerEvent::Ongoing, this, &ThisClass::Input_OnActionOngoing));
			InputBindings.Emplace(Input->BindAction(InputAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnActionCanceled));
			InputBindings.Emplace(Input->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnActionCompleted));
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

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UFuAbilityTask_InputActionListener::Pawn_OnRestarted(APawn* Pawn)
{
	UnBindActions();

	Input = Cast<UEnhancedInputComponent>(Pawn->InputComponent);

	BindActions();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnActionTriggered(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnActionTriggered.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnActionStarted(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnActionStarted.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnActionOngoing(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnActionOngoing.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnActionCanceled(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnActionCanceled.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UFuAbilityTask_InputActionListener::Input_OnActionCompleted(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnActionCompleted.Broadcast(ActionInstance.GetSourceAction(), ActionInstance.GetValue());
	}
}
