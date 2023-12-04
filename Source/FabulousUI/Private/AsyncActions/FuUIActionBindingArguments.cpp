#include "AsyncActions/FuUIActionBindingArguments.h"

#include "Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuUIActionBindingArguments)

void FFuUIActionBindingArguments::Fill(FBindUIActionArgs& ActionArguments) const
{
	ActionArguments.InputMode = InputMode;
	ActionArguments.KeyEvent = KeyEvent;
	ActionArguments.bIsPersistent = bPersistent;
	ActionArguments.bConsumeInput = bConsumeInput;
	ActionArguments.bDisplayInActionBar = bDisplayInActionBar;
	ActionArguments.OverrideDisplayName = DisplayNameOverride;
}
