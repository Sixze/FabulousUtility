#include "FuActorUtility.h"

#include "FuMacros.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuActorUtility)

bool UFuActorUtility::IsActorLoadedFromLevel(const AActor* Actor)
{
	// https://forums.unrealengine.com/t/check-if-an-actor-was-placed-in-editor-or-spawned-at-runtime/373272/7
	// https://forums.unrealengine.com/t/check-if-actor-placed-from-the-editor/301700/5

	return FU_ENSURE(IsValid(Actor)) && Actor->bNetStartup;
}

bool UFuActorUtility::TryFindComponentByClass(AActor* Actor, const TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& Component)
{
	Component = IsValid(Actor) ? Actor->FindComponentByClass(ComponentClass) : nullptr;
	return IsValid(Component);
}

void UFuActorUtility::GetPlayerViewPoint(const AActor* Actor, FVector& Location, FRotator& Rotation)
{
	Location = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator;

	if (!FU_ENSURE(IsValid(Actor)))
	{
		return;
	}

	const auto* Pawn{Cast<APawn>(Actor)};

	if (IsValid(Pawn) && IsValid(Pawn->GetController()))
	{
		Pawn->GetController()->GetPlayerViewPoint(Location, Rotation);
	}
	else
	{
		Actor->GetActorEyesViewPoint(Location, Rotation);
	}
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

	return FVector::ZeroVector;
}

FVector UFuActorUtility::GetActorFeetOffset(const AActor* Actor)
{
	const auto* Pawn{Cast<APawn>(Actor)};
	const auto* MovementComponent{IsValid(Pawn) ? Pawn->GetMovementComponent() : nullptr};

	if (IsValid(MovementComponent))
	{
		return FVector{0.0f, 0.0f, MovementComponent->UpdatedComponent->Bounds.BoxExtent.Z};
	}

	if (FU_ENSURE(IsValid(Actor)) && IsValid(Actor->GetRootComponent()))
	{
		return FVector{0.0f, 0.0f, Actor->GetRootComponent()->Bounds.BoxExtent.Z};
	}

	return FVector::ZeroVector;
}

bool UFuActorUtility::IsEncroachingBlockingGeometry(const UObject* WorldContext, const TSubclassOf<AActor> ActorClass,
                                                    const FVector& Location, const FRotator& Rotation)
{
	auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};

	return FU_ENSURE(IsValid(World)) && FU_ENSURE(IsValid(ActorClass)) &&
	       World->EncroachingBlockingGeometry(ActorClass.GetDefaultObject(), Location, Rotation);
}

bool UFuActorUtility::IsEncroachingBlockingGeometryWithAdjustment(const UObject* WorldContext, const TSubclassOf<AActor> ActorClass,
                                                                  const FVector& Location, const FRotator& Rotation,
                                                                  FVector& ProposedAdjustment)
{
	auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};

	return FU_ENSURE(IsValid(World)) && FU_ENSURE(IsValid(ActorClass)) &&
	       World->EncroachingBlockingGeometry(ActorClass.GetDefaultObject(), Location, Rotation, &ProposedAdjustment);
}
