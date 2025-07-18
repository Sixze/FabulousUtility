#include "AsyncActions/FuAsyncAction_UIActionListenerByInputAction.h"

#include "CommonUserWidget.h"
#include "FuMacros.h"
#include "Framework/Application/SlateApplication.h"
#include "Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_UIActionListenerByInputAction)

UFuAsyncAction_UIActionListenerByInputAction* UFuAsyncAction_UIActionListenerByInputAction::ListenForUIActionByInputAction(
	UCommonUserWidget* InWidget, UInputAction* InInputAction,
	const FSlateSound InSound, const FFuUIActionBindingArguments InActionArguments)
{
	auto* Task{NewObject<ThisClass>()};

	Task->Widget = InWidget;
	Task->Sound = InSound;
	Task->ActionArguments = InActionArguments;

	if (FU_ENSURE(IsValid(InInputAction)))
	{
		Task->InputActions.AddUnique(InInputAction);
	}

	return Task;
}

UFuAsyncAction_UIActionListenerByInputAction* UFuAsyncAction_UIActionListenerByInputAction::ListenForUIActionsByInputActions(
	UCommonUserWidget* InWidget, TArray<UInputAction*> InInputActions,
	const FSlateSound InSound, const FFuUIActionBindingArguments InActionArguments)
{
	auto* Task{NewObject<ThisClass>()};

	Task->Widget = InWidget;
	Task->Sound = InSound;
	Task->ActionArguments = InActionArguments;

	Task->InputActions.Reserve(InInputActions.Num());

	for (auto* InputAction : InInputActions)
	{
		if (FU_ENSURE(IsValid(InputAction)))
		{
			Task->InputActions.AddUnique(InputAction);
		}
	}

	return Task;
}

void UFuAsyncAction_UIActionListenerByInputAction::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Widget.IsValid()) || InputActions.IsEmpty())
	{
		SetReadyToDestroy();
		return;
	}

	FBindUIActionArgs ActionArgs{InputActions[0], nullptr};
	ActionArguments.Fill(ActionArgs);

	ActionHandles.Reserve(InputActions.Num());

	for (UInputAction* InputAction : InputActions)
	{
		ActionArgs.InputAction = InputAction;

		ActionArgs.OnExecuteAction = FSimpleDelegate::CreateUObject(
			this, &ThisClass::Widget_OnActionExecuted, TWeakObjectPtr{InputAction});

		const auto ActionHandle{Widget->RegisterUIActionBinding(ActionArgs)};
		if (ActionHandle.IsValid())
		{
			ActionHandles.Emplace(ActionHandle);
		}
	}

	if (ActionHandles.IsEmpty())
	{
		SetReadyToDestroy();
	}
}

void UFuAsyncAction_UIActionListenerByInputAction::SetReadyToDestroy()
{
	Widget.Reset();

	for (auto& ActionHandle : ActionHandles)
	{
		ActionHandle.Unregister();
	}

	Super::SetReadyToDestroy();
}

bool UFuAsyncAction_UIActionListenerByInputAction::ShouldBroadcastDelegates() const
{
	return Widget.IsValid();
}

void UFuAsyncAction_UIActionListenerByInputAction::Widget_OnActionExecuted(const TWeakObjectPtr<UInputAction> InputAction) const
{
	if (ShouldBroadcastDelegates())
	{
		FSlateApplication::Get().PlaySound(Sound);

		OnActionExecuted.Broadcast(InputAction.Get());
	}
}
