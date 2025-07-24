﻿#pragma once

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
	Execute(std::atomic<bool>& bExecuted, const FFuEnsureInfo& EnsureInfo);

	FABULOUSUTILITY_API bool UE_COLD UE_DEBUG_SECTION VARARGS
	ExecuteFormat(std::atomic<bool>& bExecuted, const FFuEnsureInfo& EnsureInfo, const TCHAR* Format, ...);
}

#if UE_USE_LITE_ENSURES
#define FU_ENSURE_IMPLEMENTATION(bEnsureAlways, Expression) \
	(LIKELY(Expression) || \
	 (FuEnsure::Execute(::bGEnsureHasExecuted<static_cast<uint64>(FileHashForEnsure(__FILE__)) << 32 | static_cast<uint64>(__LINE__)>, \
	                    FuEnsure::FFuEnsureInfo{#Expression, __FILE__, __LINE__, bEnsureAlways}) && \
	  BreakAndReturnFalse()))
#else
#define FU_ENSURE_IMPLEMENTATION(bEnsureAlways, Expression) \
	(LIKELY(Expression) || \
	 (FuEnsure::Execute(::bGEnsureHasExecuted<static_cast<uint64>(FileHashForEnsure(__FILE__)) << 32 | static_cast<uint64>(__LINE__)>, \
	                    FuEnsure::FFuEnsureInfo{#Expression, __FILE__, __LINE__, bEnsureAlways}) && \
	  [] \
	  { \
		  PLATFORM_BREAK(); \
		  return false; \
	  }()))
#endif

#define FU_ENSURE_IMPLEMENTATION_FORMAT(Capture, bEnsureAlways, Expression, Format, ...) \
	(LIKELY(Expression) || [Capture]() UE_COLD UE_DEBUG_SECTION \
	{ \
		static constexpr FuEnsure::FFuEnsureInfo EnsureInfo{#Expression, __builtin_FILE(), __builtin_LINE(), bEnsureAlways}; \
 		static std::atomic<bool> bExecuted{false}; \
 		\
		UE_VALIDATE_FORMAT_STRING(Format, ##__VA_ARGS__); \
		\
		if (FuEnsure::ExecuteFormat(bExecuted, EnsureInfo, Format, ##__VA_ARGS__)) \
		{ \
			PLATFORM_BREAK(); \
		} \
		\
		return false; \
	}())

#define FU_ENSURE(Expression) FU_ENSURE_IMPLEMENTATION(false, Expression)
#define FU_ENSURE_MESSAGE(Expression, Format, ...) FU_ENSURE_IMPLEMENTATION_FORMAT(&, false, Expression, Format, ##__VA_ARGS__)
#define FU_ENSURE_ALWAYS(Expression) FU_ENSURE_IMPLEMENTATION(true, Expression)
#define FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) FU_ENSURE_IMPLEMENTATION_FORMAT(&, true, Expression, Format, ##__VA_ARGS__)

#else

#define FU_ENSURE(Expression) (Expression)
#define FU_ENSURE_MESSAGE(Expression, Format, ...) (Expression)
#define FU_ENSURE_ALWAYS(Expression) (Expression)
#define FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) (Expression)

#endif
