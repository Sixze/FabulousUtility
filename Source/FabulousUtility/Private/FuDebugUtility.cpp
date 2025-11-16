#include "FuDebugUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuDebugUtility)

FORCENOINLINE FString UFuDebugUtility::GetCurrentCallstack(const EFuCallstackType CallstackType)
{
	return GetCurrentCallstackInternal(CallstackType, PLATFORM_RETURN_ADDRESS());
}

FORCENOINLINE FString UFuDebugUtility::GetCurrentCallstackBlueprint(const int32 CallstackType)
{
	return GetCurrentCallstackInternal(static_cast<EFuCallstackType>(CallstackType), PLATFORM_RETURN_ADDRESS());
}

FString UFuDebugUtility::GetCurrentCallstackInternal(const EFuCallstackType CallstackType, void* ProgramCounter)
{
	FString ResultString;
	TStringBuilder<8192> CallstackBuilder;

#if DO_BLUEPRINT_GUARD
	if (EnumHasAnyFlags(CallstackType, EFuCallstackType::ScriptCallstack))
	{
		const auto* ContextTracker{FBlueprintContextTracker::TryGet()};
		if (ContextTracker != nullptr)
		{
			const auto ScriptStack{ContextTracker->GetCurrentScriptStack()};
			if (!ScriptStack.IsEmpty())
			{
				CallstackBuilder << TEXTVIEW("Script Callstack:") LINE_TERMINATOR;

				for (auto i = ScriptStack.Num() - 1; i >= 0; i--)
				{
					ScriptStack[i]->GetStackDescription(CallstackBuilder);
					CallstackBuilder << LINE_TERMINATOR;
				}
			}
		}
	}
#endif

	if (EnumHasAnyFlags(CallstackType, EFuCallstackType::NativeCallstack))
	{
		static constexpr auto NativeCallstackSize{std::numeric_limits<uint16>::max()};
		auto* NativeCallstackString{static_cast<ANSICHAR*>(FMemory::SystemMalloc(NativeCallstackSize))};

		if (NativeCallstackString != nullptr)
		{
			FCStringAnsi::Strcpy(NativeCallstackString, "Native Callstack:" LINE_TERMINATOR_ANSI);

			FPlatformStackWalk::StackWalkAndDump(NativeCallstackString, NativeCallstackSize, ProgramCounter);

			if (CallstackBuilder.Len() <= 0)
			{
				ResultString = NativeCallstackString;
				FMemory::SystemFree(NativeCallstackString);

				return ResultString;
			}

			CallstackBuilder << NativeCallstackString;
			FMemory::SystemFree(NativeCallstackString);
		}
	}

	ResultString = CallstackBuilder;
	return ResultString;
}
