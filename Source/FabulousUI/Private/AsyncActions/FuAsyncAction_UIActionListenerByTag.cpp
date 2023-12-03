#include "AsyncActions/FuAsyncAction_UIActionListenerByTag.h"

#include "CommonUserWidget.h"
#include "FuMacros.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAsyncAction_UIActionListenerByTag)

UFuAsyncAction_UIActionListenerByTag* UFuAsyncAction_UIActionListenerByTag::ListenForUIActionByTag(
	UCommonUserWidget* InWidget, const FUIActionTag InActionTag, const ECommonInputMode InInputMode,
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

	if (FU_ENSURE(InActionTag.IsValid()))
	{
		Task->ActionTags.AddUnique(InActionTag);
	}

	return Task;
}

UFuAsyncAction_UIActionListenerByTag* UFuAsyncAction_UIActionListenerByTag::ListenForUIActionsByTags(
	UCommonUserWidget* InWidget, const FGameplayTagContainer InActionTags, const ECommonInputMode InInputMode,
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

	Task->ActionTags.Reserve(InActionTags.Num());

	for (const auto& Tag : InActionTags)
	{
		const auto ActionTag{FUIActionTag::TryConvert(Tag)};
		if (FU_ENSURE(ActionTag.IsValid()))
		{
			Task->ActionTags.AddUnique(ActionTag);
		}
	}

	return Task;
}

void UFuAsyncAction_UIActionListenerByTag::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Widget.IsValid()) || ActionTags.IsEmpty())
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

	FBindUIActionArgs ActionArguments{ActionTags[0], nullptr};
	ActionArguments.InputMode = InputMode;
	ActionArguments.KeyEvent = KeyEvent;
	ActionArguments.bIsPersistent = bPersistent;
	ActionArguments.bConsumeInput = bConsumeInput;
	ActionArguments.bDisplayInActionBar = bDisplayInActionBar;
	ActionArguments.OverrideDisplayName = DisplayNameOverride;

	ActionHandles.Reserve(ActionTags.Num());

	auto& WidgetActionHandles{const_cast<TArray<FUIActionBindingHandle>&>(Widget->GetActionBindings())};

	for (const auto& ActionTag : ActionTags)
	{
		ActionArguments.ActionTag = ActionTag;

		ActionArguments.OnExecuteAction = FSimpleDelegate::CreateUObject(this, &ThisClass::Widget_OnActionExecuted, ActionTag);

		ActionArguments.OnHoldActionProgressed = FBindUIActionArgs::FOnHoldActionProgressed::CreateUObject(
			this, &ThisClass::Widget_OnActionHeld, ActionTag);

		const auto ActionHandle{ActionRouter->RegisterUIActionBinding(*Widget.Get(), ActionArguments)};
		if (ActionHandle.IsValid())
		{
			ActionHandles.Emplace(ActionHandle);
			WidgetActionHandles.Emplace(ActionHandle);
		}
	}
}

void UFuAsyncAction_UIActionListenerByTag::Cancel()
{
	Widget.Reset();

	for (auto& ActionHandle : ActionHandles)
	{
		ActionHandle.Unregister();
	}

	Super::Cancel();
}

bool UFuAsyncAction_UIActionListenerByTag::ShouldBroadcastDelegates() const
{
	return Widget.IsValid();
}

void UFuAsyncAction_UIActionListenerByTag::Widget_OnActionExecuted(const FUIActionTag ActionTag) const
{
	if (ShouldBroadcastDelegates())
	{
		OnActionExecuted.Broadcast(ActionTag, 0.0f);
	}
}

void UFuAsyncAction_UIActionListenerByTag::Widget_OnActionHeld(const float HeldPercent, const FUIActionTag ActionTag) const
{
	if (ShouldBroadcastDelegates())
	{
		OnActionHeld.Broadcast(ActionTag, HeldPercent);
	}
}
