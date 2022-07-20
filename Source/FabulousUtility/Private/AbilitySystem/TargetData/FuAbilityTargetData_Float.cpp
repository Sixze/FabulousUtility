#include "AbilitySystem/TargetData/FuAbilityTargetData_Float.h"

UScriptStruct* FFuAbilityTargetData_Float::GetScriptStruct() const
{
	return StaticStruct();
}

FString FFuAbilityTargetData_Float::ToString() const
{
	return FU_GET_TYPE_STRING(FFuAbilityTargetData_Float);
}

bool FFuAbilityTargetData_Float::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	Archive << Value;

	bSuccess = true;
	return true;
}
