#pragma once

#include "Templates/Invoke.h"
#include "Templates/UnrealTemplate.h"

namespace FuPrivateMemberAccessor
{
	/// Writes MemberPointer into AccessorName::Pointer when its Instance is initialized.
	template <typename AccessorName, typename AccessorName::MemberType MemberPointer>
	struct TPointerInitializer
	{
		TPointerInitializer()
		{
			AccessorName::Pointer = MemberPointer;
		}

		static TPointerInitializer Instance;
	};

	/// Declaration of a TPointerInitializer instance.
	template <typename AccessorName, typename AccessorName::MemberType MemberPointer>
	TPointerInitializer<AccessorName, MemberPointer> TPointerInitializer<AccessorName, MemberPointer>::Instance;

	/// Base class of an accessor struct that provides Access(). The primary template is the generic fallback for member pointer forms
	/// without a typed specialization. Typed specializations exist for data members and for non-const and const member functions.
	template <typename AccessorName, typename MemberType>
	struct TAccessorBase
	{
		template <typename ThisType, typename... ArgumentsType>
		static decltype(auto) Access(ThisType&& This, ArgumentsType&&... Arguments)
		{
			return Invoke(AccessorName::Pointer, Forward<ThisType>(This), Forward<ArgumentsType>(Arguments)...);
		}
	};

	/// Typed Access() for data members. The requires clause keeps member function pointers that match no
	/// other specialization, such as ref-qualified ones, on the generic fallback of the primary template.
	template <typename AccessorName, typename ClassType, typename ResultType> requires
		(!std::is_member_function_pointer_v<ResultType ClassType::*>)
	struct TAccessorBase<AccessorName, ResultType ClassType::*>
	{
		static ResultType& Access(ClassType& This)
		{
			return Invoke(AccessorName::Pointer, This);
		}

		static const ResultType& Access(const ClassType& This)
		{
			return Invoke(AccessorName::Pointer, This);
		}

		static ResultType& Access(ClassType* This)
		{
			return Invoke(AccessorName::Pointer, This);
		}

		static const ResultType& Access(const ClassType* This)
		{
			return Invoke(AccessorName::Pointer, This);
		}
	};

	/// Typed Access() for non-const member functions.
	template <typename AccessorName, typename ClassType, typename ResultType, typename... ArgumentsType>
	struct TAccessorBase<AccessorName, ResultType (ClassType::*)(ArgumentsType...)>
	{
		static ResultType Access(ClassType& This, ArgumentsType... Arguments)
		{
			return Invoke(AccessorName::Pointer, This, Forward<ArgumentsType>(Arguments)...);
		}

		static ResultType Access(ClassType* This, ArgumentsType... Arguments)
		{
			return Invoke(AccessorName::Pointer, This, Forward<ArgumentsType>(Arguments)...);
		}
	};

	/// Typed Access() for const member functions.
	template <typename AccessorName, typename ClassType, typename ResultType, typename... ArgumentsType>
	struct TAccessorBase<AccessorName, ResultType (ClassType::*)(ArgumentsType...) const>
	{
		static ResultType Access(const ClassType& This, ArgumentsType... Arguments)
		{
			return Invoke(AccessorName::Pointer, This, Forward<ArgumentsType>(Arguments)...);
		}

		static ResultType Access(const ClassType* This, ArgumentsType... Arguments)
		{
			return Invoke(AccessorName::Pointer, This, Forward<ArgumentsType>(Arguments)...);
		}
	};
}

/// Alternative to UE_DEFINE_PRIVATE_MEMBER_PTR() that works with overloaded functions. Type is written in the plain or function type form.
#define FU_DEFINE_PRIVATE_MEMBER_ACCESSOR(AccessorName, Class, Member, ...) \
	struct AccessorName : public FuPrivateMemberAccessor::TAccessorBase<AccessorName, TIdentity_T<__VA_ARGS__> Class::*> \
	{ \
		using MemberType = TIdentity_T<__VA_ARGS__> Class::*; \
		\
		static inline MemberType Pointer; \
	}; \
	\
	template struct FuPrivateMemberAccessor::TPointerInitializer<AccessorName, &Class::Member>;
