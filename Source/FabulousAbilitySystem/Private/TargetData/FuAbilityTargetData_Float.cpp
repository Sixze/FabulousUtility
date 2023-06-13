#include "TargetData/FuAbilityTargetData_Float.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTargetData_Float)

UScriptStruct* FFuAbilityTargetData_Float::GetScriptStruct() const
{
	return StaticStruct();
}

FString FFuAbilityTargetData_Float::ToString() const
{
	return FString{FU_GET_TYPE_STRING(FFuAbilityTargetData_Float)};
}

bool FFuAbilityTargetData_Float::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bSuccess)
{
	Archive << Value;

	bSuccess = true;
	return true;
}
