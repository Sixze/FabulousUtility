#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

constexpr const TCHAR* UFuWorldUtility::WorldTypeToString(const EWorldType::Type WorldType)
{
	if (WorldType < EWorldType::None || WorldType > EWorldType::Inactive)
	{
		return TEXT("Unknown World Type");
	}

	static const TCHAR* Strings[]
	{
		FU_GET_ENUM_VALUE_STRING(EWorldType, None),
		FU_GET_ENUM_VALUE_STRING(EWorldType, Game),
		FU_GET_ENUM_VALUE_STRING(EWorldType, Editor),
		FU_GET_ENUM_VALUE_STRING(EWorldType, PIE),
		FU_GET_ENUM_VALUE_STRING(EWorldType, EditorPreview),
		FU_GET_ENUM_VALUE_STRING(EWorldType, GamePreview),
		FU_GET_ENUM_VALUE_STRING(EWorldType, GameRPC),
		FU_GET_ENUM_VALUE_STRING(EWorldType, Inactive)
	};

	return Strings[WorldType];
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{WorldContext->GetWorld()};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
