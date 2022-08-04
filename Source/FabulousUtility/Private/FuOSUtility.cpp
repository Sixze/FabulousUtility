#include "FuOSUtility.h"

#if PLATFORM_WINDOWS
#include "FuMacros.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Windows/WindowsHWrapper.h"
#endif

void UFuOSUtility::FlashUnfocusedWindow()
{
#if PLATFORM_WINDOWS
	auto* Viewport{IsValid(GEngine) ? GEngine->GameViewport.Get() : nullptr};
	const auto Window{IsValid(Viewport) ? Viewport->GetWindow() : nullptr};
	const auto NativeWindow{Window.IsValid() ? Window->GetNativeWindow() : nullptr};
	const auto WindowHandle{NativeWindow.IsValid() ? static_cast<HWND>(NativeWindow->GetOSWindowHandle()) : nullptr};

	if (FU_ENSURE(IsWindow(WindowHandle)) && WindowHandle != GetForegroundWindow())
	{
		FlashWindow(WindowHandle, true);
		MessageBeep(MB_ICONINFORMATION);
	}
#endif
}
