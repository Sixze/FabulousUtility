﻿#pragma once

#include <type_traits>

namespace FuPrivateMemberAccessor
{
	// Global pointer to a field or method.
	template <typename AccessorName>
	typename AccessorName::MemberType TMemberPointer;

	// Struct that is used to initialize TPointer.
	template <typename AccessorName, typename AccessorName::MemberType MemberPointer>
	struct TPointerInitializer
	{
		TPointerInitializer()
		{
			TMemberPointer<AccessorName> = MemberPointer;
		}

		static TPointerInitializer Instance;
	};

	// Declaration of a TPointerInitializer instance.
	template <typename AccessorName, typename AccessorName::MemberType MemberPointer>
	TPointerInitializer<AccessorName, MemberPointer> TPointerInitializer<AccessorName, MemberPointer>::Instance;

	// Returns the value of the data member referenced by this accessor.
	template <typename AccessorName, typename ThisType>
	decltype(auto) Access(ThisType&& This)
	{
		if constexpr (std::is_pointer_v<ThisType>)
		{
			return This->*TMemberPointer<AccessorName>;
		}
		else
		{
			return This.*TMemberPointer<AccessorName>;
		}
	}

	// Invokes the member function referenced by this accessor.
	template <typename AccessorName, typename ThisType, typename... ArgumentsType>
	decltype(auto) Invoke(ThisType&& This, ArgumentsType&&... Arguments)
	{
		if constexpr (std::is_pointer_v<ThisType>)
		{
			return (This->*TMemberPointer<AccessorName>)(Forward<ArgumentsType>(Arguments)...);
		}
		else
		{
			return (This.*TMemberPointer<AccessorName>)(Forward<ArgumentsType>(Arguments)...);
		}
	}
}

// Alternative to UE_DEFINE_PRIVATE_MEMBER_PTR() that works with overloaded functions.
#define FU_DEFINE_PRIVATE_MEMBER_ACCESSOR(AccessorName, MemberPointer, ...) \
	struct AccessorName \
	{ \
		using MemberType = __VA_ARGS__; \
		\
		template <typename ThisType> \
		static decltype(auto) Access(ThisType&& This) \
		{ \
			return FuPrivateMemberAccessor::Access<AccessorName>(Forward<ThisType>(This)); \
		} \
		\
		template <typename ThisType, typename... ArgumentsType> \
		static decltype(auto) Invoke(ThisType&& This, ArgumentsType&&... Arguments) \
		{ \
			return FuPrivateMemberAccessor::Invoke<AccessorName>(Forward<ThisType>(This), Forward<ArgumentsType>(Arguments)...); \
		} \
	}; \
	\
	template struct FuPrivateMemberAccessor::TPointerInitializer<AccessorName, MemberPointer>;
