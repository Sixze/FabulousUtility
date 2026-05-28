#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuWorldUtility)

namespace FuWorldUtility
{
	constexpr FAnsiStringView WorldTypeStrings[]
	{
		ANSITEXTVIEW("None"),
		ANSITEXTVIEW("Game"),
		ANSITEXTVIEW("Editor"),
		ANSITEXTVIEW("PIE"),
		ANSITEXTVIEW("Editor Preview"),
		ANSITEXTVIEW("Game Preview"),
		ANSITEXTVIEW("Game RPC"),
		ANSITEXTVIEW("Inactive")
	};
}

constexpr FAnsiStringView UFuWorldUtility::WorldTypeToString(const EWorldType::Type WorldType)
{
	return WorldType >= EWorldType::None && WorldType <= EWorldType::Inactive
		       ? FuWorldUtility::WorldTypeStrings[WorldType]
		       : ANSITEXTVIEW("Unknown");
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
