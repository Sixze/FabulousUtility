#include "AsyncActions/FuAsyncAction_UIActionListenerByInputAction.h"

#include "CommonUserWidget.h"
#include "FuMacros.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_UIActionListenerByInputAction)

UFuAsyncAction_UIActionListenerByInputAction* UFuAsyncAction_UIActionListenerByInputAction::ListenForUIActionByInputAction(
	UCommonUserWidget* InWidget, UInputAction* InInputAction, const ECommonInputMode InInputMode,
	const TEnumAsByte<EInputEvent> InKeyEvent, const bool bInPersistent, const bool bInConsumeInput,
	const bool bInDisplayInActionBar, const FText InDisplayNameOverride)
{
	auto* Task{NewObject<ThisClass>()};

	Task->Widget = InWidget;
	Task->InputMode = InInputMode;
	Task->KeyEvent = InKeyEvent;
	Task->bPersistent = bInPersistent;
	Task->bConsumeInput = bInConsumeInput;
	Task->bDisplayInActionBar = bInDisplayInActionBar;
	Task->DisplayNameOverride = InDisplayNameOverride;

	if (FU_ENSURE(IsValid(InInputAction)))
	{
		Task->InputActions.AddUnique(InInputAction);
	}

	return Task;
}

UFuAsyncAction_UIActionListenerByInputAction* UFuAsyncAction_UIActionListenerByInputAction::ListenForUIActionsByInputActions(
	UCommonUserWidget* InWidget, TArray<UInputAction*> InInputActions, const ECommonInputMode InInputMode,
	const TEnumAsByte<EInputEvent> InKeyEvent, const bool bInPersistent, const bool bInConsumeInput,
	const bool bInDisplayInActionBar, const FText InDisplayNameOverride)
{
	auto* Task{NewObject<ThisClass>()};

	Task->Widget = InWidget;
	Task->InputMode = InInputMode;
	Task->KeyEvent = InKeyEvent;
	Task->bPersistent = bInPersistent;
	Task->bConsumeInput = bInConsumeInput;
	Task->bDisplayInActionBar = bInDisplayInActionBar;
	Task->DisplayNameOverride = InDisplayNameOverride;

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

	// Here, it is preferable to use the UCommonUserWidget::RegisterUIActionBinding() and UCommonUserWidget::RemoveActionBinding()
	// functions to add and remove action bindings, but currently this is not possible because they are protected.

	auto* ActionRouter{UCommonUIActionRouterBase::Get(*Widget.Get())};
	if (!IsValid(ActionRouter))
	{
		SetReadyToDestroy();
		return;
	}

	FBindUIActionArgs ActionArguments{InputActions[0], nullptr};
	ActionArguments.InputMode = InputMode;
	ActionArguments.KeyEvent = KeyEvent;
	ActionArguments.bIsPersistent = bPersistent;
	ActionArguments.bConsumeInput = bConsumeInput;
	ActionArguments.bDisplayInActionBar = bDisplayInActionBar;
	ActionArguments.OverrideDisplayName = DisplayNameOverride;

	ActionHandles.Reserve(InputActions.Num());

	auto& WidgetActionHandles{const_cast<TArray<FUIActionBindingHandle>&>(Widget->GetActionBindings())};

	for (UInputAction* InputAction : InputActions)
	{
		ActionArguments.InputAction = InputAction;

		ActionArguments.OnExecuteAction = FSimpleDelegate::CreateUObject(
			this, &ThisClass::Widget_OnActionExecuted, TWeakObjectPtr<UInputAction>{InputAction});

		const auto ActionHandle{ActionRouter->RegisterUIActionBinding(*Widget.Get(), ActionArguments)};
		if (ActionHandle.IsValid())
		{
			ActionHandles.Emplace(ActionHandle);
			WidgetActionHandles.Emplace(ActionHandle);
		}
	}
}

void UFuAsyncAction_UIActionListenerByInputAction::Cancel()
{
	Widget.Reset();

	for (auto& ActionHandle : ActionHandles)
	{
		ActionHandle.Unregister();
	}

	Super::Cancel();
}

bool UFuAsyncAction_UIActionListenerByInputAction::ShouldBroadcastDelegates() const
{
	return Widget.IsValid();
}

void UFuAsyncAction_UIActionListenerByInputAction::Widget_OnActionExecuted(const TWeakObjectPtr<UInputAction> InputAction) const
{
	if (ShouldBroadcastDelegates())
	{
		OnActionExecuted.Broadcast(InputAction.Get());
	}
}
