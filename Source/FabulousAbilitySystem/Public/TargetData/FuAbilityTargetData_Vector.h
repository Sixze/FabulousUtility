#pragma once

#include "FuMacros.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilityTargetData_Vector.generated.h"

USTRUCT(BlueprintType, DisplayName = "Fu Vector Target Data")
struct FABULOUSABILITYSYSTEM_API FFuAbilityTargetData_Vector : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FVector Vector{ForceInit};

public:
	virtual bool HasOrigin() const override;

	virtual FTransform GetOrigin() const override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToString() const override;

	bool NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess);
};

template <>
struct TStructOpsTypeTraits<FFuAbilityTargetData_Vector> : public TStructOpsTypeTraitsBase2<FFuAbilityTargetData_Vector>
{
	enum
	{
		WithNetSerializer = true
	};
};

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuVectorTargetDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector Target Data Utility",
		Meta = (AutoCreateRefTerm = "TargetData", ReturnDisplayName = "Target Data Handle"))
	static FGameplayAbilityTargetDataHandle MakeVectorTargetDataHandle(
		const FFuAbilityTargetData_Vector& TargetData);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Vector Target Data Utility",
		Meta = (ReturnDisplayName = "Target Data"))
	static FFuAbilityTargetData_Vector GetVectorTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, int32 Index);
};

inline FGameplayAbilityTargetDataHandle UFuVectorTargetDataUtility::MakeVectorTargetDataHandle(
	const FFuAbilityTargetData_Vector& TargetData)
{
	return {new FFuAbilityTargetData_Vector{TargetData}};
}

inline FFuAbilityTargetData_Vector UFuVectorTargetDataUtility::GetVectorTargetData(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle, const int32 Index)
{
	return TargetDataHandle.Data.IsValidIndex(Index) &&
	       FU_ENSURE(TargetDataHandle.Data[Index].Get()->GetScriptStruct()->IsChildOf(FFuAbilityTargetData_Vector::StaticStruct()))
		       ? *static_cast<FFuAbilityTargetData_Vector*>(TargetDataHandle.Data[Index].Get())
		       : FFuAbilityTargetData_Vector{};
}
