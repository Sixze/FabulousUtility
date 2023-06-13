#include "TargetData/FuAbilityTargetData_Vector.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTargetData_Vector)

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
	return FString{FU_GET_TYPE_STRING(FFuAbilityTargetData_Vector)};
}

bool FFuAbilityTargetData_Vector::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	bSuccess = true;

	bSuccess &= SerializePackedVector<100, 30>(Vector, Archive);

	return bSuccess;
}
