#include "FuControllerUtility.h"

#include "FuMacros.h"
#include "GameFramework/Pawn.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(FuControllerUtility)

AController* UFuControllerUtility::GetControllerFromActor(AActor* Actor)
{
	if (!FU_ENSURE(IsValid(Actor)))
	{
		return nullptr;
	}

	const auto* Pawn{Cast<APawn>(Actor)};
	if (IsValid(Pawn))
	{
		return Pawn->GetController();
	}

	return Cast<AController>(Actor);
}

bool UFuControllerUtility::TryGetControllerFromActor(AActor* Actor, AController*& Controller)
{
	const auto* Pawn{Cast<APawn>(Actor)};

	Controller = IsValid(Pawn) ? Pawn->GetController() : nullptr;
	if (IsValid(Controller))
	{
		return true;
	}

	Controller = Cast<AController>(Actor);
	return IsValid(Controller);
}
