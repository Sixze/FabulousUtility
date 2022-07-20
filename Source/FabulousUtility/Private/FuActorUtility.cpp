#include "FuActorUtility.h"

#include "FuMacros.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"

bool UFuActorUtility::TryGetComponentByClass(AActor* Actor, const TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& Component)
{
	Component = FU_ENSURE(IsValid(Actor)) ? Actor->FindComponentByClass(ComponentClass) : nullptr;
	return IsValid(Component);
}

FVector UFuActorUtility::GetActorFeetLocation(const AActor* Actor)
{
	const auto* Pawn{Cast<APawn>(Actor)};

	return IsValid(Pawn)
		       ? Pawn->GetMovementComponent()->GetActorFeetLocation()
		       : FU_ENSURE(IsValid(Actor)) && FU_ENSURE(IsValid(Actor->GetRootComponent()))
		       ? Actor->GetRootComponent()->GetComponentLocation() - FVector{0.0, 0.0, Actor->GetRootComponent()->Bounds.BoxExtent.Z}
		       : FNavigationSystem::InvalidLocation;
}

FVector UFuActorUtility::GetActorFeetOffset(const AActor* Actor)
{
	const auto* Pawn{Cast<APawn>(Actor)};

	return IsValid(Pawn)
		       ? FVector{0.0, 0.0, Pawn->GetMovementComponent()->UpdatedComponent->Bounds.BoxExtent.Z}
		       : FU_ENSURE(IsValid(Actor)) && FU_ENSURE(IsValid(Actor->GetRootComponent()))
		       ? FVector{0.0, 0.0, Actor->GetRootComponent()->Bounds.BoxExtent.Z}
		       : FVector::ZeroVector;
}
