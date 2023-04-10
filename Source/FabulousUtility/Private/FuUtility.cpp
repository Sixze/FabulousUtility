#include "FuUtility.h"

#include "FuMacros.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuUtility)

UGameInstance* UFuUtility::GetGameInstanceCasted(const UObject* WorldContext, const TSubclassOf<UGameInstance> GameInstanceClass)
{
	const auto* World{WorldContext->GetWorld()};
	auto* GameInstance{FU_ENSURE(IsValid(World)) ? World->GetGameInstance() : nullptr};

	return FU_ENSURE(IsValid(GameInstance)) && FU_ENSURE(IsValid(GameInstanceClass)) && GameInstance->IsA(GameInstanceClass)
		       ? GameInstance
		       : nullptr;
}

bool UFuUtility::TryGetGameInstanceCasted(const UObject* WorldContext, const TSubclassOf<UGameInstance> GameInstanceClass,
                                          UGameInstance*& GameInstance)
{
	const auto* World{WorldContext->GetWorld()};
	GameInstance = IsValid(World) ? World->GetGameInstance() : nullptr;

	if (!IsValid(GameInstance) || !IsValid(GameInstanceClass) || !GameInstance->IsA(GameInstanceClass))
	{
		GameInstance = nullptr;
		return false;
	}

	return true;
}
