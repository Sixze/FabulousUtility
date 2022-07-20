#pragma once

#include "FuMacros.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilityTargetData_Float.generated.h"

USTRUCT(BlueprintType, DisplayName = "Fu Float Target Data")
struct FABULOUSUTILITY_API FFuAbilityTargetData_Float : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value{0.0f};

public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToString() const override;

	bool NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess);
};

template <>
struct TStructOpsTypeTraits<FFuAbilityTargetData_Float> : public TStructOpsTypeTraitsBase2<FFuAbilityTargetData_Float>
{
	enum
	{
		WithNetSerializer = true
	};
};

UCLASS()
class FABULOUSUTILITY_API UFuFloatTargetDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Float Target Data Utility", Meta = (AutoCreateRefTerm = "TargetData"))
	static FGameplayAbilityTargetDataHandle MakeFloatTargetDataHandle(const FFuAbilityTargetData_Float& TargetData);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Float Target Data Utility")
	static FFuAbilityTargetData_Float GetFloatTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, int32 Index);
};

inline FGameplayAbilityTargetDataHandle UFuFloatTargetDataUtility::MakeFloatTargetDataHandle(const FFuAbilityTargetData_Float& TargetData)
{
	return {new FFuAbilityTargetData_Float{TargetData}};
}

inline FFuAbilityTargetData_Float UFuFloatTargetDataUtility::GetFloatTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
                                                                                const int32 Index)
{
	return TargetDataHandle.Data.IsValidIndex(Index) &&
	       FU_ENSURE(TargetDataHandle.Data[Index].Get()->GetScriptStruct()->IsChildOf(FFuAbilityTargetData_Float::StaticStruct()))
		       ? *static_cast<FFuAbilityTargetData_Float*>(TargetDataHandle.Data[Index].Get())
		       : FFuAbilityTargetData_Float{};
}
