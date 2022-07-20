#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

constexpr const TCHAR* UFuWorldUtility::WorldTypeToString(const EWorldType::Type WorldType)
{
	switch (WorldType)
	{
		case EWorldType::None:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, None);
		case EWorldType::Game:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, Game);
		case EWorldType::Editor:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, Editor);
		case EWorldType::PIE:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, PIE);
		case EWorldType::EditorPreview:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, EditorPreview);
		case EWorldType::GamePreview:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, GamePreview);
		case EWorldType::GameRPC:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, GameRPC);
		case EWorldType::Inactive:
			return FU_GET_ENUM_VALUE_STRING(EWorldType, Inactive);
		default:
			return TEXT("Unknown");
	}
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{WorldContext->GetWorld()};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
