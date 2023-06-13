#pragma once

#include "FuMacros.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilityTargetData_LocationAndRotation.generated.h"

USTRUCT(BlueprintType, DisplayName = "Fu Location and Rotation Target Data")
struct FABULOUSABILITYSYSTEM_API FFuAbilityTargetData_LocationAndRotation : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FVector Location{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabulous Utility")
	FRotator Rotation{ForceInit};

public:
	virtual bool HasOrigin() const override;

	virtual FTransform GetOrigin() const override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToString() const override;

	bool NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess);
};

template <>
struct TStructOpsTypeTraits<FFuAbilityTargetData_LocationAndRotation> :
	public TStructOpsTypeTraitsBase2<FFuAbilityTargetData_LocationAndRotation>
{
	enum
	{
		WithNetSerializer = true
	};
};

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuLocationAndRotationTargetDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Location and Rotation Target Data Utility",
		Meta = (AutoCreateRefTerm = "TargetData", ReturnDisplayName = "Target Data Handle"))
	static FGameplayAbilityTargetDataHandle MakeLocationAndRotationTargetDataHandle(
		const FFuAbilityTargetData_LocationAndRotation& TargetData);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Location and Rotation Target Data Utility",
		Meta = (ReturnDisplayName = "Target Data"))
	static FFuAbilityTargetData_LocationAndRotation GetLocationAndRotationTargetData(
		const FGameplayAbilityTargetDataHandle& TargetDataHandle, int32 Index);
};

inline FGameplayAbilityTargetDataHandle UFuLocationAndRotationTargetDataUtility::MakeLocationAndRotationTargetDataHandle(
	const FFuAbilityTargetData_LocationAndRotation& TargetData)
{
	return {new FFuAbilityTargetData_LocationAndRotation{TargetData}};
}

inline FFuAbilityTargetData_LocationAndRotation UFuLocationAndRotationTargetDataUtility::GetLocationAndRotationTargetData(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle, const int32 Index)
{
	return TargetDataHandle.Data.IsValidIndex(Index) && FU_ENSURE(
		       TargetDataHandle.Data[Index].Get()->GetScriptStruct()->IsChildOf(FFuAbilityTargetData_LocationAndRotation::StaticStruct()))
		       ? *static_cast<FFuAbilityTargetData_LocationAndRotation*>(TargetDataHandle.Data[Index].Get())
		       : FFuAbilityTargetData_LocationAndRotation{};
}
