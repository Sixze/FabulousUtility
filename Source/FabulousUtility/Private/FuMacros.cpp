#include "FuMacros.h"

#include "CoreGlobals.h"
#include "Async/UniqueLock.h"
#include "Async/WordMutex.h"
#include "Containers/StaticArray.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformTime.h"
#include "Misc/CoreDelegates.h"
#include "Templates/Function.h"

#if DO_ENSURE && !USING_CODE_ANALYSIS

namespace FuEnsure
{
	static bool CanExecute(std::atomic<uint8>& bExecuted, const FFuEnsureInfo& EnsureInfo)
	{
		static const auto* EnsureStateConsoleVariable{
			IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsureState"))
		};
		check(EnsureStateConsoleVariable != nullptr)

		static const auto* EnsureAlwaysEnabledConsoleVariable{
			IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsureAlwaysEnabled"))
		};
		check(EnsureAlwaysEnabledConsoleVariable != nullptr)

		const auto EnsureState{EnsureStateConsoleVariable->GetInt()};

		if ((bExecuted.load(std::memory_order_relaxed) == EnsureState &&
		     (!EnsureInfo.bEnsureAlways || !EnsureAlwaysEnabledConsoleVariable->GetBool())) ||
		    !FPlatformMisc::IsEnsureAllowed())
		{
			return false;
		}

		return bExecuted.exchange(EnsureState, std::memory_order_release) != EnsureState || EnsureInfo.bEnsureAlways;
	}

	static bool ExecuteInternal(const FFuEnsureInfo& EnsureInfo, const TCHAR* Message)
	{
		if (UNLIKELY(GetEnsureHandler() && GetEnsureHandler()({EnsureInfo.Expression, Message})))
		{
			return false;
		}

		if (FPlatformTime::GetSecondsPerCycle() != 0.0f)
		{
			TStringBuilder<512> EnsureBuilder{
				InPlace, ANSITEXTVIEW("Ensure failed: "), EnsureInfo.Expression, ANSITEXTVIEW(", File: "),
				EnsureInfo.FilePath ? EnsureInfo.FilePath : "Unknown", ANSITEXTVIEW(", Line: "), EnsureInfo.LineNumber, ANSITEXTVIEW(".")
			};

			FCoreDelegates::OnEnsureFailed.Broadcast(EnsureInfo.Expression, EnsureInfo.FilePath,
			                                         EnsureInfo.LineNumber, Message, EnsureBuilder.ToString());

			static const auto* EnsuresAreErrorsConsoleVariable{
				IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsuresAreErrors"))
			};
			check(EnsuresAreErrorsConsoleVariable != nullptr)

#if !NO_LOGGING
			if (EnsuresAreErrorsConsoleVariable->GetBool())
			{
				UE_LOGF(LogOutputDevice, Error, "%ls", EnsureBuilder.ToString())
				UE_LOGF(LogOutputDevice, Error, "%ls", Message)
			}
			else
			{
				UE_LOGF(LogOutputDevice, Warning, "%ls", EnsureBuilder.ToString())
				UE_LOGF(LogOutputDevice, Warning, "%ls", Message)
			}
#endif

			PrintScriptCallstack();
		}

		if (!FPlatformMisc::IsDebuggerPresent())
		{
			FPlatformMisc::PromptForRemoteDebugging(true);
			return false;
		}

		static const auto* EnsureBreakEnabledConsoleVariable{
			IConsoleManager::Get().FindConsoleVariable(TEXT("core.EnsureBreakEnabled"))
		};
		check(EnsureBreakEnabledConsoleVariable != nullptr)

#if UE_BUILD_SHIPPING
		return EnsureBreakEnabledConsoleVariable->GetBool();
#else
		return !GIgnoreDebugger && EnsureBreakEnabledConsoleVariable->GetBool();
#endif
	}

	bool UE_COLD UE_DEBUG_SECTION Execute(std::atomic<uint8>& bExecuted, const FFuEnsureInfo& EnsureInfo)
	{
		return CanExecute(bExecuted, EnsureInfo) && ExecuteInternal(EnsureInfo, TEXT(""));
	}

	bool UE_COLD UE_DEBUG_SECTION ExecuteFormat(std::atomic<uint8>& bExecuted, const FFuEnsureInfo& EnsureInfo,
	                                            const TCHAR* Format, ...)
	{
		if (!CanExecute(bExecuted, EnsureInfo))
		{
			return false;
		}

		static UE::FWordMutex FormatMutex;
		static constexpr auto MessageSize{std::numeric_limits<uint16>::max()};
		static TStaticArray<TCHAR, MessageSize> Message;

		// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
		UE::TUniqueLock Lock{FormatMutex};

		GET_TYPED_VARARGS(TCHAR, Message.GetData(), MessageSize, FormattedMessageSize - 1, Format, Format);

		return ExecuteInternal(EnsureInfo, Message.GetData());
	}
}

#endif
