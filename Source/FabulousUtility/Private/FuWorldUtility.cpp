#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuWorldUtility)

namespace FuWorldUtilityConstants
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
		       ? FuWorldUtilityConstants::WorldTypeStrings[WorldType]
		       : TEXTVIEW("Unknown World Type");
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{WorldContext->GetWorld()};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
