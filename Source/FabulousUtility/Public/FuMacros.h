#pragma once

#include "Misc/AssertionMacros.h"

#define FU_GET_TYPE_STRING(Type) \
	((void) sizeof UEAsserts_Private::GetMemberNameCheckedJunk(static_cast<Type*>(nullptr)), TEXTVIEW(#Type))

// A lightweight version of the ensure() macro that doesn't generate a C++ call stack and doesn't send a
// crash report, because it doesn't happen instantly and causes the editor to freeze, which can be annoying.

#if DO_ENSURE && !USING_CODE_ANALYSIS

namespace FuEnsure
{
	struct FFuEnsureInfo
	{
		const ANSICHAR* Expression{nullptr};

		const ANSICHAR* FilePath{nullptr};

		int32 LineNumber{0};

		uint8 bEnsureAlways : 1 {false};
	};

	FABULOUSUTILITY_API bool UE_COLD UE_DEBUG_SECTION VARARGS
	Execute(std::atomic<bool>& bExecuted, const FFuEnsureInfo& EnsureInfo, const TCHAR* Format, ...);
}

#define FU_ENSURE_IMPLEMENTATION(Capture, bEnsureAlways, Expression, Format, ...) \
	(LIKELY(Expression) || [Capture]() UE_COLD UE_DEBUG_SECTION \
	{ \
		static constexpr FuEnsure::FFuEnsureInfo EnsureInfo{#Expression, __builtin_FILE(), __builtin_LINE(), bEnsureAlways}; \
 		static std::atomic<bool> bExecuted{false}; \
 		\
		UE_VALIDATE_FORMAT_STRING(Format, ##__VA_ARGS__); \
		\
		if (FuEnsure::Execute(bExecuted, EnsureInfo, Format, ##__VA_ARGS__)) \
		{ \
			PLATFORM_BREAK(); \
		} \
		\
		return false; \
	}())

#define FU_ENSURE(Expression) FU_ENSURE_IMPLEMENTATION( , false, Expression, TEXT(""))
#define FU_ENSURE_MESSAGE(Expression, Format, ...) FU_ENSURE_IMPLEMENTATION(&, false, Expression, Format, ##__VA_ARGS__)
#define FU_ENSURE_ALWAYS(Expression) FU_ENSURE_IMPLEMENTATION( , true, Expression, TEXT(""))
#define FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) FU_ENSURE_IMPLEMENTATION(&, true, Expression, Format, ##__VA_ARGS__)

#else

#define FU_ENSURE(Expression) (Expression)
#define FU_ENSURE_MESSAGE(Expression, Format, ...) (Expression)
#define FU_ENSURE_ALWAYS(Expression) (Expression)
#define FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) (Expression)

#endif
