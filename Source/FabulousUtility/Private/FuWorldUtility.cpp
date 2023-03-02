#include "FuWorldUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(FuWorldUtility)

constexpr FStringView UFuWorldUtility::WorldTypeToString(const EWorldType::Type WorldType)
{
	if (WorldType < EWorldType::None || WorldType > EWorldType::Inactive)
	{
		return TEXTVIEW("Unknown World Type");
	}

	static constexpr FStringView Strings[]
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

	return Strings[WorldType];
}

float UFuWorldUtility::GetWorldGravityZ(const UObject* WorldContext)
{
	const auto* World{WorldContext->GetWorld()};

	return FU_ENSURE(IsValid(World)) ? World->GetGravityZ() : 0.0f;
}
