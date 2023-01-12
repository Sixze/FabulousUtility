#include "UI/FuCommonUIUtility.h"

#include "CommonInputSubsystem.h"

ECommonInputType UFuCommonUIUtility::GetInputTypeForKey(const FKey& Key)
{
	// Based on FCommonInputPreprocessor::GetInputType().

	return !Key.IsGamepadKey()
		       ? ECommonInputType::MouseAndKeyboard
		       : !UCommonInputSubsystem::IsMobileGamepadKey(Key)
		       ? ECommonInputType::Gamepad
		       : ECommonInputType::Touch;
}
