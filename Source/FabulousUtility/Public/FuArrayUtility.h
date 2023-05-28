#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Array Utility", Meta = (ReturnDisplayName = "Random Index"))
	static int32 GetWeightedRandomIndex(const TArray<float>& Array);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Array Utility", Meta = (AutoCreateRefTerm = "Array"))
	static void SortByPredicateObject(UPARAM(ref) TArray<UObject*>& Objects, const FFuSortByPredicatObjectDelegate& Predicate);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Array Utility", Meta = (AutoCreateRefTerm = "Array"))
	static void SortByPredicateActor(UPARAM(ref) TArray<AActor*>& Actors, const FFuSortByPredicateActorDelegate& Predicate);

	// Internal blueprint only functions.

private:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Array Utility",
		CustomThunk, Meta = (ArrayParm = "Array", ReturnDisplayName = "Value"))
	static bool IsEmpty(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execIsEmpty);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Array Utility",
		CustomThunk, Meta = (ArrayParm = "Array", ReturnDisplayName = "Value"))
	static bool IsNotEmpty(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execIsNotEmpty);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Array Utility",
		CustomThunk, Meta = (ArrayParm = "Array", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsEmpty(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execSwitchIsEmpty);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Array Utility",
		CustomThunk, Meta = (ArrayParm = "Array", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsNotEmpty(const TArray<UObject*>& Array);

	DECLARE_FUNCTION(execSwitchIsNotEmpty);

	static bool IsEmpty(void* Array, const FArrayProperty* Property);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Array Utility", CustomThunk, Meta = (ArrayParm = "Array"))
	static void Shuffle(UPARAM(ref) TArray<int32>& Array);

	DECLARE_FUNCTION(execShuffle);

	static void Shuffle(void* Array, const FArrayProperty* Property);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Array Utility", CustomThunk, Meta = (ArrayParm = "Array"))
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

inline bool UFuArrayUtility::SwitchIsEmpty(const TArray<UObject*>& Array)
{
	checkNoEntry()
	return true;
}

inline bool UFuArrayUtility::SwitchIsNotEmpty(const TArray<UObject*>& Array)
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
