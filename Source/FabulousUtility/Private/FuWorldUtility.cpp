#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuWorldUtility)

constexpr FAnsiStringView UFuWorldUtility::WorldTypeToString(const EWorldType::Type WorldType)
{
	switch (WorldType)
	{
		case EWorldType::None:
			return ANSITEXTVIEW("None");

		case EWorldType::Game:
			return ANSITEXTVIEW("Game");

		case EWorldType::Editor:
			return ANSITEXTVIEW("Editor");

		case EWorldType::PIE:
			return ANSITEXTVIEW("PIE");

		case EWorldType::EditorPreview:
			return ANSITEXTVIEW("Editor Preview");

		case EWorldType::GamePreview:
			return ANSITEXTVIEW("Game Preview");

		case EWorldType::GameRPC:
			return ANSITEXTVIEW("Game RPC");

		case EWorldType::Inactive:
			return ANSITEXTVIEW("Inactive");

		default:
			return ANSITEXTVIEW("Unknown");
	}
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
