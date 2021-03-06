#include "AbilitySystem/TargetData/FuAbilityTargetData_Vector.h"

bool FFuAbilityTargetData_Vector::HasOrigin() const
{
	return true;
}

FTransform FFuAbilityTargetData_Vector::GetOrigin() const
{
	return FTransform{Vector};
}

UScriptStruct* FFuAbilityTargetData_Vector::GetScriptStruct() const
{
	return StaticStruct();
}

FString FFuAbilityTargetData_Vector::ToString() const
{
	return FU_GET_TYPE_STRING(FFuAbilityTargetData_Vector);
}

bool FFuAbilityTargetData_Vector::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	bSuccess = true;

	bSuccess &= SerializePackedVector<100, 30>(Vector, Archive);

	return bSuccess;
}
