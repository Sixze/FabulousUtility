#include "UI/AsyncActions/FuAsyncAction_UIActionListener.h"

#include "CommonUserWidget.h"
#include "FuMacros.h"
#include "Input/CommonUIActionRouterBase.h"

UFuAsyncAction_UIActionListener* UFuAsyncAction_UIActionListener::FuListenForUIAction(
	UCommonUserWidget* Widget, const FUIActionTag ActionTag, const FFuUIActionBindingArguments ActionArguments)
{
	auto* Task{NewObject<ThisClass>()};

	Task->Widget1 = Widget;
	Task->ActionArguments1 = ActionArguments;

	if (FU_ENSURE(ActionTag.IsValid()))
	{
		Task->ActionTags1.AddUnique(ActionTag);
	}

	return Task;
}

UFuAsyncAction_UIActionListener* UFuAsyncAction_UIActionListener::FuListenForUIActions(
	UCommonUserWidget* Widget, const FGameplayTagContainer ActionTags, const FFuUIActionBindingArguments ActionArguments)
{
	auto* Task{NewObject<ThisClass>()};

	Task->Widget1 = Widget;
	Task->ActionArguments1 = ActionArguments;

	for (const auto& Tag : ActionTags)
	{
		const auto ActionTag{FUIActionTag::TryConvert(Tag)};
		if (FU_ENSURE(ActionTag.IsValid()))
		{
			Task->ActionTags1.AddUnique(ActionTag);
		}
	}

	return Task;
}

void UFuAsyncAction_UIActionListener::Activate()
{
	Super::Activate();

	if (!FU_ENSURE(Widget1.IsValid()) || ActionTags1.IsEmpty())
	{
		SetReadyToDestroy();
		return;
	}

	// Here, it is preferable to use the UCommonUserWidget::RegisterUIActionBinding() and UCommonUserWidget::RemoveActionBinding()
	// functions to add and remove action bindings, but currently this is not possible because they are protected.

	auto* ActionRouter{UCommonUIActionRouterBase::Get(*Widget1.Get())};
	if (!IsValid(ActionRouter))
	{
		SetReadyToDestroy();
		return;
	}

	FBindUIActionArgs ActionArguments{ActionTags1[0], nullptr};
	ActionArguments.InputMode = ActionArguments1.InputMode;
	ActionArguments.KeyEvent = ActionArguments1.KeyEvent;
	ActionArguments.bIsPersistent = ActionArguments1.bIsPersistent;
	ActionArguments.bConsumeInput = ActionArguments1.bConsumeInput;
	ActionArguments.bDisplayInActionBar = ActionArguments1.bDisplayInActionBar;
	ActionArguments.OverrideDisplayName = ActionArguments1.DisplayNameOverride;

	auto& WidgetActionHandles{const_cast<TArray<FUIActionBindingHandle>&>(Widget1->GetActionBindings())};

	for (const auto& ActionTag : ActionTags1)
	{
		ActionArguments.ActionTag = ActionTag;

		ActionArguments.OnExecuteAction = FSimpleDelegate::CreateUObject(this, &ThisClass::Widget_OnActionExecuted, ActionTag);

		ActionArguments.OnHoldActionProgressed = FBindUIActionArgs::FOnHoldActionProgressed::CreateUObject(
			this, &ThisClass::Widget_OnActionHeld, ActionTag);

		const auto ActionHandle{ActionRouter->RegisterUIActionBinding(*Widget1.Get(), ActionArguments)};
		if (ActionHandle.IsValid())
		{
			ActionHandles.Add(ActionHandle);
			WidgetActionHandles.Add(ActionHandle);
		}
	}
}

void UFuAsyncAction_UIActionListener::Cancel()
{
	if (Widget1.IsValid())
	{
		auto* ActionRouter{UCommonUIActionRouterBase::Get(*Widget1.Get())};
		auto& WidgetActionHandles{const_cast<TArray<FUIActionBindingHandle>&>(Widget1->GetActionBindings())};

		for (const auto& ActionHandle : ActionHandles)
		{
			WidgetActionHandles.Remove(ActionHandle);

			if (IsValid(ActionRouter))
			{
				ActionRouter->RemoveBinding(ActionHandle);
			}
		}
	}

	for (auto& ActionHandle : ActionHandles)
	{
		ActionHandle.Unregister();
	}

	Super::Cancel();
}

bool UFuAsyncAction_UIActionListener::ShouldBroadcastDelegates() const
{
	return Widget1.IsValid();
}

void UFuAsyncAction_UIActionListener::Widget_OnActionExecuted(const FUIActionTag ActionTag) const
{
	if (ShouldBroadcastDelegates())
	{
		OnActionExecuted.Broadcast(ActionTag, 0.0f);
	}
}

void UFuAsyncAction_UIActionListener::Widget_OnActionHeld(const float HeldPercent, const FUIActionTag ActionTag) const
{
	if (ShouldBroadcastDelegates())
	{
		OnActionHeld.Broadcast(ActionTag, HeldPercent);
	}
}
