#include "AbilitySystem/TargetData/FuAbilityTargetData_LocationAndRotation.h"

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
	return FU_GET_TYPE_STRING(FFuAbilityTargetData_LocationAndRotation);
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
