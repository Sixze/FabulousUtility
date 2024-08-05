#pragma once

#include "Misc/AssertionMacros.h"

#define FU_STRINGIFY_IMPLEMENTATION(Value) #Value

#define FU_STRINGIFY(Value) FU_STRINGIFY_IMPLEMENTATION(Value)

#define FU_GET_TYPE_STRING(Type) \
	((void) sizeof UEAsserts_Private::GetMemberNameCheckedJunk(static_cast<Type*>(nullptr)), TEXTVIEW(#Type))

// A lightweight version of the ensure() macro that doesn't generate a C++ call stack and doesn't send a
// crash report, because it doesn't happen instantly and causes the editor to freeze, which can be annoying.

#if DO_ENSURE && !USING_CODE_ANALYSIS

namespace FuEnsure
{
	FABULOUSUTILITY_API bool UE_DEBUG_SECTION VARARGS Execute(std::atomic<bool>& bExecuted, bool bEnsureAlways, const ANSICHAR* Expression,
	                                                          const TCHAR* StaticMessage, const TCHAR* Format, ...);
}

#define FU_ENSURE_IMPLEMENTATION(Capture, bEnsureAlways, Expression, Format, ...) \
	(LIKELY(Expression) || [Capture]() UE_DEBUG_SECTION \
	{ \
		static constexpr auto StaticMessage{TEXT("Ensure failed: " #Expression ", File: " __FILE__ ", Line: " FU_STRINGIFY(__LINE__) ".")}; \
 		static std::atomic<bool> bExecuted{false}; \
 		\
		UE_VALIDATE_FORMAT_STRING(Format, ##__VA_ARGS__); \
		\
		if (FuEnsure::Execute(bExecuted, bEnsureAlways, #Expression, StaticMessage, Format, ##__VA_ARGS__)) \
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
