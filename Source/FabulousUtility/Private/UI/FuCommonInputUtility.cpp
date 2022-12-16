#include "UI/FuCommonInputUtility.h"

#include "CommonInputSubsystem.h"

ECommonInputType UFuCommonInputUtility::GetInputTypeForKey(const FKey& Key)
{
	// Based on FCommonInputPreprocessor::GetInputType().

	return !Key.IsGamepadKey()
		       ? ECommonInputType::MouseAndKeyboard
		       : !UCommonInputSubsystem::IsMobileGamepadKey(Key)
		       ? ECommonInputType::Gamepad
		       : ECommonInputType::Touch;
}
