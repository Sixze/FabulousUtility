#include "FuActorUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"
#include "GameFramework/PawnMovementComponent.h"

bool UFuActorUtility::TryGetComponentByClass(AActor* Actor, const TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& Component)
{
	Component = FU_ENSURE(IsValid(Actor)) ? Actor->FindComponentByClass(ComponentClass) : nullptr;
	return IsValid(Component);
}

FVector UFuActorUtility::GetActorFeetLocation(const AActor* Actor)
{
	const auto* Pawn{Cast<APawn>(Actor)};
	const auto* MovementComponent{IsValid(Pawn) ? Pawn->GetMovementComponent() : nullptr};

	if (IsValid(MovementComponent))
	{
		return MovementComponent->GetActorFeetLocation();
	}

	if (FU_ENSURE(IsValid(Actor) && IsValid(Actor->GetRootComponent())))
	{
		return Actor->GetRootComponent()->GetComponentLocation() - FVector{0.0f, 0.0f, Actor->GetRootComponent()->Bounds.BoxExtent.Z};
	}

	return FNavigationSystem::InvalidLocation;
}

FVector UFuActorUtility::GetActorFeetOffset(const AActor* Actor)
{
	const auto* Pawn{Cast<APawn>(Actor)};
	const auto* MovementComponent{IsValid(Pawn) ? Pawn->GetMovementComponent() : nullptr};

	if (IsValid(MovementComponent))
	{
		return FVector{0.0f, 0.0f, MovementComponent->UpdatedComponent->Bounds.BoxExtent.Z};
	}

	if (FU_ENSURE(IsValid(Actor) && IsValid(Actor->GetRootComponent())))
	{
		return FVector{0.0f, 0.0f, Actor->GetRootComponent()->Bounds.BoxExtent.Z};
	}

	return FVector::ZeroVector;
}

bool UFuActorUtility::IsEncroachingBlockingGeometry(const UObject* WorldContext, const TSubclassOf<AActor> ActorClass,
                                                    const FVector& Location, const FRotator& Rotation)
{
	auto* World{WorldContext->GetWorld()};

	return FU_ENSURE(IsValid(World)) && FU_ENSURE(IsValid(ActorClass)) &&
	       World->EncroachingBlockingGeometry(ActorClass->GetDefaultObject<AActor>(), Location, Rotation);
}

bool UFuActorUtility::IsEncroachingBlockingGeometryWithAdjustment(const UObject* WorldContext, const TSubclassOf<AActor> ActorClass,
                                                                  const FVector& Location, const FRotator& Rotation,
                                                                  FVector& ProposedAdjustment)
{
	auto* World{WorldContext->GetWorld()};

	return FU_ENSURE(IsValid(World)) && FU_ENSURE(IsValid(ActorClass)) &&
	       World->EncroachingBlockingGeometry(ActorClass->GetDefaultObject<AActor>(), Location, Rotation, &ProposedAdjustment);
}
