#include "TargetData/FuAbilityTargetData_LocationAndRotation.h"

#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTargetData_LocationAndRotation)

bool FFuAbilityTargetData_LocationAndRotation::HasOrigin() const
{
	return true;
}

FTransform FFuAbilityTargetData_LocationAndRotation::GetOrigin() const
{
	return {Rotation, Location};
}

UScriptStruct* FFuAbilityTargetData_LocationAndRotation::GetScriptStruct() const
{
	return StaticStruct();
}

FString FFuAbilityTargetData_LocationAndRotation::ToString() const
{
	return FString{FU_GET_TYPE_STRING(FFuAbilityTargetData_LocationAndRotation)};
}

bool FFuAbilityTargetData_LocationAndRotation::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	bSuccess = true;
	auto bSuccessLocal{true};

	bSuccess &= SerializePackedVector<100, 30>(Location, Archive);

	Rotation.NetSerialize(Archive, Map, bSuccessLocal);
	Rotation.Normalize();
	bSuccess &= bSuccessLocal;

	return bSuccess;
}

FGameplayAbilityTargetDataHandle UFuLocationAndRotationTargetDataUtility::MakeLocationAndRotationTargetData(
	const FVector& Location, const FRotator& Rotation)
{
	auto TargetData{MakeShared<FFuAbilityTargetData_LocationAndRotation>()};
	TargetData->Location = Location;
	TargetData->Rotation = Rotation;

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Data.Emplace(MoveTemp(TargetData));

	return TargetDataHandle;
}

FFuAbilityTargetData_LocationAndRotation UFuLocationAndRotationTargetDataUtility::GetLocationAndRotationTargetData(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle, const int32 TargetDataIndex)
{
	if (!TargetDataHandle.Data.IsValidIndex(TargetDataIndex) ||
	    !FU_ENSURE(TargetDataHandle.Data[TargetDataIndex].Get()->GetScriptStruct()
		    ->IsChildOf(FFuAbilityTargetData_LocationAndRotation::StaticStruct())))
	{
		return {};
	}

	return *static_cast<FFuAbilityTargetData_LocationAndRotation*>(TargetDataHandle.Data[TargetDataIndex].Get());
}
