#include "FuArrayUtility.h"

#include "Net/Core/PushModel/PushModel.h"

DEFINE_FUNCTION(UFuArrayUtility::execShuffle)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);

	void* ArrayAddress{Stack.MostRecentPropertyAddress};
	const auto* ArrayProperty{CastField<FArrayProperty>(Stack.MostRecentProperty)};

	if (ArrayProperty == nullptr)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_FINISH

	P_NATIVE_BEGIN
		MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty)
		Shuffle(ArrayAddress, ArrayProperty);
	P_NATIVE_END
}

void UFuArrayUtility::Shuffle(void* Array, const FArrayProperty* Property)
{
	if (Array == nullptr)
	{
		return;
	}

	FScriptArrayHelper ArrayHelper{Property, Array};
	const auto LastIndex{ArrayHelper.Num() - 1};

	for (auto i{0}; i <= LastIndex; i++)
	{
		const auto NewIndex{FMath::RandRange(i, LastIndex)};
		if (i != NewIndex)
		{
			ArrayHelper.SwapValues(i, NewIndex);
		}
	}
}

DEFINE_FUNCTION(UFuArrayUtility::execShuffleFirstElements)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);

	void* ArrayAddress{Stack.MostRecentPropertyAddress};
	const auto* ArrayProperty{CastField<FArrayProperty>(Stack.MostRecentProperty)};

	if (ArrayProperty == nullptr)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_GET_PROPERTY(FIntProperty, FirstElementsCount)
	P_FINISH

	P_NATIVE_BEGIN
		MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty)
		ShuffleFirstElements(ArrayAddress, ArrayProperty, FirstElementsCount);
	P_NATIVE_END
}

void UFuArrayUtility::ShuffleFirstElements(void* Array, const FArrayProperty* Property, const int32 FirstElementsCount)
{
	if (Array == nullptr)
	{
		return;
	}

	FScriptArrayHelper ArrayHelper{Property, Array};
	const auto LastIndex{ArrayHelper.Num() - 1};
	const auto LastElementIndex{FMath::Min(LastIndex, FirstElementsCount - 1)};

	for (auto i{0}; i <= LastElementIndex; i++)
	{
		const auto NewIndex{FMath::RandRange(i, LastIndex)};
		if (i != NewIndex)
		{
			ArrayHelper.SwapValues(i, NewIndex);
		}
	}
}

DEFINE_FUNCTION(UFuArrayUtility::execIsEmpty)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);

	void* ArrayAddress{Stack.MostRecentPropertyAddress};
	const auto* ArrayProperty{CastField<FArrayProperty>(Stack.MostRecentProperty)};

	if (ArrayProperty == nullptr)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_FINISH

	P_NATIVE_BEGIN
		*static_cast<bool*>(RESULT_PARAM) = IsEmpty(ArrayAddress, ArrayProperty);
	P_NATIVE_END
}

DEFINE_FUNCTION(UFuArrayUtility::execIsNotEmpty)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);

	void* ArrayAddress{Stack.MostRecentPropertyAddress};
	const auto* ArrayProperty{CastField<FArrayProperty>(Stack.MostRecentProperty)};

	if (ArrayProperty == nullptr)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_FINISH

	P_NATIVE_BEGIN
		*static_cast<bool*>(RESULT_PARAM) = !IsEmpty(ArrayAddress, ArrayProperty);
	P_NATIVE_END
}

DEFINE_FUNCTION(UFuArrayUtility::execSwitchIsEmpty)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);

	void* ArrayAddress{Stack.MostRecentPropertyAddress};
	const auto* ArrayProperty{CastField<FArrayProperty>(Stack.MostRecentProperty)};

	if (ArrayProperty == nullptr)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_FINISH

	P_NATIVE_BEGIN
		*static_cast<bool*>(RESULT_PARAM) = IsEmpty(ArrayAddress, ArrayProperty);
	P_NATIVE_END
}

DEFINE_FUNCTION(UFuArrayUtility::execSwitchIsNotEmpty)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);

	void* ArrayAddress{Stack.MostRecentPropertyAddress};
	const auto* ArrayProperty{CastField<FArrayProperty>(Stack.MostRecentProperty)};

	if (ArrayProperty == nullptr)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_FINISH

	P_NATIVE_BEGIN
		*static_cast<bool*>(RESULT_PARAM) = !IsEmpty(ArrayAddress, ArrayProperty);
	P_NATIVE_END
}

bool UFuArrayUtility::IsEmpty(void* Array, const FArrayProperty* Property)
{
	if (Array == nullptr)
	{
		return true;
	}

	const FScriptArrayHelper ArrayHelper{Property, Array};

	return ArrayHelper.Num() <= 0;
}
