#include "Input/FuPlayerInput.h"

#include "Components/InputComponent.h"

void UFuPlayerInput::RetriggerPressedActionsAndKeys(const TArray<UInputComponent*>& InputStack,
                                                    const bool bBlockedInputComponentsOnly) const
{
	TriggerInputEventForPressedActionsAndKeys(InputStack, IE_Pressed, bBlockedInputComponentsOnly);
}

void UFuPlayerInput::ReleasePressedActionsAndKeys(const TArray<UInputComponent*>& InputStack, const bool bBlockedInputComponentsOnly) const
{
	TriggerInputEventForPressedActionsAndKeys(InputStack, IE_Released, bBlockedInputComponentsOnly);
}

void UFuPlayerInput::TriggerInputEventForPressedActionsAndKeys(const TArray<UInputComponent*>& InputStack,
                                                               const EInputEvent InputEvent, const bool bBlockedInputComponentsOnly) const
{
	// Based on UEnhancedPlayerInput::ProcessInputStack().

	static TArray<FKey> PressedKeys;
	check(PressedKeys.IsEmpty())

	for (const auto& [Key, KeyState] : const_cast<ThisClass*>(this)->GetKeyStateMap())
	{
		if (KeyState.bDown ||
		    KeyState.EventCounts[IE_Pressed].Num() > 0 ||
		    KeyState.EventCounts[IE_Repeat].Num() > 0)
		{
			PressedKeys.Add(Key);
		}
	}

	if (PressedKeys.IsEmpty())
	{
		return;
	}

	// Based on UPlayerInput::ProcessInputStack().

	auto bInputIsBlockedForTheRestOfTheInputStack{false};

	const auto bAlt{IsAltPressed()};
	const auto bCtrl{IsCtrlPressed()};
	const auto bShift{IsShiftPressed()};
	const auto bCmd{IsCmdPressed()};

	static TArray<const FInputActionUnifiedDelegate*> Delegates;
	check(Delegates.IsEmpty())

	for (auto i{InputStack.Num() - 1}; i >= 0; i--)
	{
		const auto* Input{InputStack[i]};

		// ReSharper disable once CppRedundantParentheses
		if (!IsValid(Input) || (bBlockedInputComponentsOnly && !bInputIsBlockedForTheRestOfTheInputStack))
		{
			bInputIsBlockedForTheRestOfTheInputStack |= Input->bBlockInput;
			continue;
		}

		for (auto ActionIndex{0}; ActionIndex < Input->GetNumActionBindings(); ActionIndex++)
		{
			const auto& ActionBinding{Input->GetActionBinding(ActionIndex)};

			if (ActionBinding.KeyEvent != InputEvent)
			{
				continue;
			}

			for (const auto& KeyMapping : GetKeysForAction(ActionBinding.GetActionName()))
			{
				if ((KeyMapping.Key == EKeys::AnyKey || PressedKeys.Contains(KeyMapping.Key)) &&
				    (bAlt || !KeyMapping.bAlt) &&
				    (bCtrl || !KeyMapping.bCtrl) &&
				    (bShift || !KeyMapping.bShift) &&
				    (bCmd || !KeyMapping.bCmd) &&
				    ActionBinding.ActionDelegate.IsBound())
				{
					Delegates.Add(&ActionBinding.ActionDelegate);
				}
			}
		}

		for (const auto& KeyBinding : Input->KeyBindings)
		{
			if (KeyBinding.KeyEvent == InputEvent &&
			    (KeyBinding.Chord.Key == EKeys::AnyKey || PressedKeys.Contains(KeyBinding.Chord.Key)) &&
			    (bAlt || !KeyBinding.Chord.bAlt) &&
			    (bCtrl || !KeyBinding.Chord.bCtrl) &&
			    (bShift || !KeyBinding.Chord.bShift) &&
			    (bCmd || !KeyBinding.Chord.bCmd) &&
			    KeyBinding.KeyDelegate.IsBound())
			{
				Delegates.Add(&KeyBinding.KeyDelegate);
			}
		}

		bInputIsBlockedForTheRestOfTheInputStack |= Input->bBlockInput;
	}

	for (const auto* Delegate : Delegates)
	{
		Delegate->Execute(EKeys::Invalid);
	}

	PressedKeys.Reset();
	Delegates.Reset();
}
