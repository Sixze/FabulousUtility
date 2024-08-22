#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/EnumerateRange.h"
#include "FuArrayUtility.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FFuSortByPredicatObjectDelegate, const UObject*, A, const UObject*, B);

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FFuSortByPredicateActorDelegate, const AActor*, A, const AActor*, B);

UCLASS()
class FABULOUSUTILITY_API UFuArrayUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename ValueType>
	static void Shuffle(TArray<ValueType>& Array);

	template <typename ValueType>
	static void ShuffleFirstElements(TArray<ValueType>& Array, int32 FirstElementsCount);

	template <typename ValueType, typename PredicateType> requires std::is_invocable_r_v<float, PredicateType, ValueType>
	static int32 GetWeightedRandomIndexByPredicate(const TArray<ValueType>& Array, const PredicateType& WeightPredicate);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Array Utility", Meta = (ReturnDisplayName = "Random Index"))
	static int32 GetWeightedRandomIndex(const TArray<float>& Array);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Array Utility", Meta = (AutoCreateRefTerm = "Array"))
	static void SortByPredicateObject(UPARAM(ref) TArray<UObject*>& Objects, const FFuSortByPredicatObjectDelegate& Predicate);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Array Utility", Meta = (AutoCreateRefTerm = "Array"))
	static void SortByPredicateActor(UPARAM(ref) TArray<AActor*>& Actors, const FFuSortByPredicateActorDelegate& Predicate);

	// Internal blueprint-only functions.

private:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Array Utility",
		CustomThunk, Meta = (ArrayParm = "Array", ReturnDisplayName = "Value"))
	static bool IsEmpty(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execIsEmpty);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Array Utility",
		CustomThunk, Meta = (ArrayParm = "Array", ReturnDisplayName = "Value"))
	static bool IsNotEmpty(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execIsNotEmpty);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Array Utility", CustomThunk,
		DisplayName = "Is Empty (Expanded)", Meta = (ArrayParm = "Array", ExpandBoolAsExecs = "ReturnValue"))
	static bool IsEmptyExpanded(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execIsEmptyExpanded);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Array Utility", CustomThunk,
		DisplayName = "Is Not Empty (Expanded)", Meta = (ArrayParm = "Array", ExpandBoolAsExecs = "ReturnValue"))
	static bool IsNotEmptyExpanded(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execIsNotEmptyExpanded);

	static bool IsEmpty(void* Array, const FArrayProperty* Property);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Array Utility", CustomThunk, Meta = (ArrayParm = "Array"))
	static void Shuffle(UPARAM(ref) TArray<int32>& Array);

	DECLARE_FUNCTION(execShuffle);

	static void Shuffle(void* Array, const FArrayProperty* Property);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Array Utility", CustomThunk, Meta = (ArrayParm = "Array"))
	static void ShuffleFirstElements(UPARAM(ref) TArray<int32>& Array, int32 FirstElementsCount);

	DECLARE_FUNCTION(execShuffleFirstElements);

	static void ShuffleFirstElements(void* Array, const FArrayProperty* Property, int32 FirstElementsCount);
};

template <typename ValueType>
void UFuArrayUtility::Shuffle(TArray<ValueType>& Array)
{
	const auto LastIndex{Array.Num() - 1};

	for (auto i{0}; i <= LastIndex; i++)
	{
		const auto NewIndex{FMath::RandRange(i, LastIndex)};
		if (i != NewIndex)
		{
			Array.SwapMemory(i, NewIndex);
		}
	}
}

template <typename ValueType>
void UFuArrayUtility::ShuffleFirstElements(TArray<ValueType>& Array, const int32 FirstElementsCount)
{
	const auto LastIndex{Array.Num() - 1};
	const auto LastElementIndex{FMath::Min(LastIndex, FirstElementsCount - 1)};

	for (auto i{0}; i <= LastElementIndex; i++)
	{
		const auto NewIndex{FMath::RandRange(i, LastIndex)};
		if (i != NewIndex)
		{
			Array.SwapMemory(i, NewIndex);
		}
	}
}

template <typename ValueType, typename PredicateType> requires std::is_invocable_r_v<float, PredicateType, ValueType>
int32 UFuArrayUtility::GetWeightedRandomIndexByPredicate(const TArray<ValueType>& Array, const PredicateType& WeightPredicate)
{
	if (Array.IsEmpty())
	{
		return -1;
	}

	auto TotalWeight{0.0f};

	for (const ValueType& ArrayItem : Array)
	{
		TotalWeight += Invoke(WeightPredicate, ArrayItem);
	}

	const auto RandomWeight{FMath::FRand() * TotalWeight};
	auto ActualWeight{0.0f};

	for (const auto Weight : EnumerateRange(Array))
	{
		ActualWeight += Invoke(WeightPredicate, *Weight);

		if (RandomWeight <= ActualWeight)
		{
			return Weight.GetIndex();
		}
	}

	return -1;
}

inline bool UFuArrayUtility::IsEmpty(const TArray<UObject*>& Array)
{
	checkNoEntry()
	return true;
}

inline bool UFuArrayUtility::IsNotEmpty(const TArray<UObject*>& Array)
{
	checkNoEntry()
	return true;
}

inline bool UFuArrayUtility::IsEmptyExpanded(const TArray<UObject*>& Array)
{
	checkNoEntry()
	return true;
}

inline bool UFuArrayUtility::IsNotEmptyExpanded(const TArray<UObject*>& Array)
{
	checkNoEntry()
	return true;
}

inline void UFuArrayUtility::Shuffle(TArray<int32>& Array)
{
	checkNoEntry()
}

inline void UFuArrayUtility::ShuffleFirstElements(TArray<int32>& Array, const int32 FirstElementsCount)
{
	checkNoEntry()
}
