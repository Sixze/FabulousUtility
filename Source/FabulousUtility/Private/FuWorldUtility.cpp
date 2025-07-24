#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuWorldUtility)

namespace FuWorldUtility
{
	constexpr FStringView WorldTypeStrings[]
	{
		TEXTVIEW("None"),
		TEXTVIEW("Game"),
		TEXTVIEW("Editor"),
		TEXTVIEW("PIE"),
		TEXTVIEW("Editor Preview"),
		TEXTVIEW("Game Preview"),
		TEXTVIEW("Game RPC"),
		TEXTVIEW("Inactive")
	};
}

constexpr FStringView UFuWorldUtility::WorldTypeToString(const EWorldType::Type WorldType)
{
	return WorldType >= EWorldType::None && WorldType <= EWorldType::Inactive
		       ? FuWorldUtility::WorldTypeStrings[WorldType]
		       : TEXTVIEW("Unknown World Type");
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
