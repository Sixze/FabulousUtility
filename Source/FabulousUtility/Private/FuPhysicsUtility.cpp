#include "FuPhysicsUtility.h"

#include "FuMacros.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuPhysicsUtility)

void UFuPhysicsUtility::FindReachableActorsInRadius(const UObject* WorldContext, const FVector& Location,
                                                    const float Radius, const FCollisionProfileName& CollisionProfile,
                                                    const TFunctionRef<bool(const FOverlapResult& Overlap)> FilterPredicate,
                                                    TMap<AActor*, TArray<FHitResult>>& ReachableActors, const AActor* IgnoredActor)
{
	ECollisionChannel CollisionChannel;
	FCollisionResponseParams CollisionResponses;
	if (!UCollisionProfile::GetChannelAndResponseParams(CollisionProfile.Name, CollisionChannel, CollisionResponses))
	{
		ReachableActors.Reset();
		return;
	}

	return FindReachableActorsInRadius(WorldContext, Location, Radius, CollisionChannel, CollisionResponses, FilterPredicate,
	                                   ReachableActors, IgnoredActor);
}

void UFuPhysicsUtility::FindReachableActorsInRadius(const UObject* WorldContext, const FVector& Location, float Radius,
                                                    ECollisionChannel CollisionChannel, const FCollisionResponseParams& CollisionResponses,
                                                    TFunctionRef<bool(const FOverlapResult& Overlap)> FilterPredicate,
                                                    TMap<AActor*, TArray<FHitResult>>& ReachableActors, const AActor* IgnoredActor)
{
	// Based on UGameplayStatics::ApplyRadialDamageWithFalloff().

	ReachableActors.Reset();

	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	if (!FU_ENSURE(IsValid(World)))
	{
		return;
	}

	TArray<FOverlapResult> Overlaps;

	World->OverlapMultiByChannel(Overlaps, Location, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(Radius),
	                             {__FUNCTION__, false, IgnoredActor}, CollisionResponses);

	FHitResult Hit;

	for (const auto& Overlap : Overlaps)
	{
		auto* Actor{Overlap.GetActor()};
		if (!IsValid(Actor))
		{
			continue;
		}

		auto* ActorHits{ReachableActors.Find(Actor)};

		if ((ActorHits == nullptr && !FilterPredicate(Overlap)) || !Overlap.Component.IsValid() ||
		    !IsComponentReachableFromLocation(Overlap.Component.Get(), Location, CollisionChannel, CollisionResponses, Hit, IgnoredActor))
		{
			continue;
		}

		if (ActorHits == nullptr)
		{
			ActorHits = &ReachableActors.Emplace(Actor);
		}

		ActorHits->Emplace(Hit);
	}
}

bool UFuPhysicsUtility::IsComponentReachableFromLocation(UPrimitiveComponent* Component, const FVector& Location,
                                                         const ECollisionChannel CollisionChannel,
                                                         const FCollisionResponseParams& CollisionResponses,
                                                         FHitResult& Hit, const AActor* IgnoredActor)
{
	if (!FU_ENSURE(IsValid(Component)))
	{
		Hit.Init();
		return false;
	}

	const auto& ComponentBounds{Component->Bounds};

	static const FVector TraceStartOffset{0.0f, 0.0f, 50.0f};
	const FVector TraceEndOffset{0.0f, 0.0f, ComponentBounds.BoxExtent.Z};

	return IsComponentReachableByLineTrace(Component, Location,
	                                       ComponentBounds.Origin,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location,
	                                       ComponentBounds.Origin + TraceEndOffset,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location,
	                                       ComponentBounds.Origin - TraceEndOffset,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location + TraceStartOffset,
	                                       ComponentBounds.Origin,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location + TraceStartOffset,
	                                       ComponentBounds.Origin + TraceEndOffset,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location + TraceStartOffset,
	                                       ComponentBounds.Origin - TraceEndOffset,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location - TraceStartOffset,
	                                       ComponentBounds.Origin,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location - TraceStartOffset,
	                                       ComponentBounds.Origin + TraceEndOffset,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor) ||
	       IsComponentReachableByLineTrace(Component, Location - TraceStartOffset,
	                                       ComponentBounds.Origin - TraceEndOffset,
	                                       CollisionChannel, CollisionResponses, Hit, IgnoredActor);
}

bool UFuPhysicsUtility::IsComponentReachableByLineTrace(UPrimitiveComponent* Component, const FVector& TraceStart,
                                                        const FVector& TraceEnd, const ECollisionChannel CollisionChannel,
                                                        const FCollisionResponseParams& CollisionResponses,
                                                        FHitResult& Hit, const AActor* IgnoredActor)
{
	// Based on ComponentIsDamageableFrom().

	if (!FU_ENSURE(IsValid(Component)))
	{
		Hit.Init();
		return false;
	}

	if (Component->GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, CollisionChannel,
	                                                    {__FUNCTION__, true, IgnoredActor}, CollisionResponses))
	{
		return Hit.Component.Get() == Component;
	}

	const auto FakeHitLocation{Component->GetComponentLocation()};
	const auto FakeHitNormal{(TraceStart - FakeHitLocation).GetSafeNormal()};
	Hit = {Component->GetOwner(), Component, FakeHitLocation, FakeHitNormal};

	return true;
}

bool UFuPhysicsUtility::LineTraceBone(UPrimitiveComponent* Primitive, const FName BoneName,
                                      const FVector& TraceStart, const FVector& TraceEnd, FHitResult& Hit)
{
	if (!FU_ENSURE(IsValid(Primitive)))
	{
		Hit.Init();
		return false;
	}

	const auto* Body{Primitive->GetBodyInstance(BoneName)};

	return Body != nullptr && Body->LineTrace(Hit, TraceStart, TraceEnd, false, true);
}

bool UFuPhysicsUtility::BoxOverlapActors(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
                                         const FVector& Extent, const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
                                         const TArray<AActor*>& IgnoreActors, TArray<AActor*>& Actors)
{
	Actors.Reset();

	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	if (!FU_ENSURE(IsValid(World)))
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto CollisionChannel : CollisionChannels)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(CollisionChannel);
	}

	FCollisionQueryParams QueryParameters{__FUNCTION__, false};
	QueryParameters.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;

	World->OverlapMultiByObjectType(Overlaps, Location, Rotation.Quaternion(), ObjectQueryParameters,
	                                FCollisionShape::MakeBox(Extent), QueryParameters);

	for (const auto& Overlap : Overlaps)
	{
		auto* Actor{Overlap.GetActor()};

		if (IsValid(Actor) && !Actors.Contains(Actor))
		{
			Actors.Emplace(Actor);
		}
	}

	return !Actors.IsEmpty();
}

bool UFuPhysicsUtility::BoxOverlapComponents(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
                                             const FVector& Extent, const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
                                             const TArray<AActor*>& IgnoreActors, TArray<UPrimitiveComponent*>& Components)
{
	Components.Reset();

	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	if (!FU_ENSURE(IsValid(World)))
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto CollisionChannel : CollisionChannels)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(CollisionChannel);
	}

	FCollisionQueryParams QueryParameters{__FUNCTION__, false};
	QueryParameters.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;

	World->OverlapMultiByObjectType(Overlaps, Location, Rotation.Quaternion(), ObjectQueryParameters,
	                                FCollisionShape::MakeBox(Extent), QueryParameters);

	for (const auto& Overlap : Overlaps)
	{
		if (Overlap.Component.IsValid())
		{
			Components.Emplace(Overlap.Component.Get());
		}
	}

	return !Components.IsEmpty();
}

bool UFuPhysicsUtility::ConeOverlapActorsSimple(const UObject* WorldContext, const FVector& Location,
                                                const FRotator& Rotation, const float Radius, const float Angle,
                                                const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
                                                const TArray<AActor*>& IgnoreActors, TArray<AActor*>& Actors)
{
	Actors.Reset();

	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	if (!FU_ENSURE(IsValid(World)))
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto CollisionChannel : CollisionChannels)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(CollisionChannel);
	}

	FCollisionQueryParams QueryParameters{__FUNCTION__, false};
	QueryParameters.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;

	World->OverlapMultiByObjectType(Overlaps, Location, Rotation.Quaternion(), ObjectQueryParameters,
	                                FCollisionShape::MakeSphere(Radius), QueryParameters);

	const auto Direction{Rotation.Vector()};
	const auto AngleCos{FMath::Cos(FMath::DegreesToRadians(FMath::Clamp(Angle, 0.0f, 180.0f)))};

	for (const auto& Overlap : Overlaps)
	{
		auto* Actor{Overlap.GetActor()};

		if (IsValid(Actor) && !Actors.Contains(Actor) &&
		    (Direction | (Overlap.Component->GetComponentLocation() - Location).GetSafeNormal()) >= AngleCos)
		{
			Actors.Emplace(Actor);
		}
	}
	return !Actors.IsEmpty();
}

bool UFuPhysicsUtility::ConeOverlapComponentsSimple(const UObject* WorldContext, const FVector& Location,
                                                    const FRotator& Rotation, const float Radius, const float Angle,
                                                    const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
                                                    const TArray<AActor*>& IgnoreActors, TArray<UPrimitiveComponent*>& Components)
{
	Components.Reset();

	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	if (!FU_ENSURE(IsValid(World)))
	{
		return false;
	}

	FCollisionObjectQueryParams ObjectQueryParameters;
	for (const auto CollisionChannel : CollisionChannels)
	{
		ObjectQueryParameters.AddObjectTypesToQuery(CollisionChannel);
	}

	FCollisionQueryParams QueryParameters{__FUNCTION__, false};
	QueryParameters.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;

	World->OverlapMultiByObjectType(Overlaps, Location, Rotation.Quaternion(), ObjectQueryParameters,
	                                FCollisionShape::MakeSphere(Radius), QueryParameters);

	const auto Direction{Rotation.Vector()};
	const auto AngleCos{FMath::Cos(FMath::DegreesToRadians(FMath::Clamp(Angle, 0.0f, 180.0f)))};

	for (const auto& Overlap : Overlaps)
	{
		if (Overlap.Component.IsValid() &&
		    (Direction | (Overlap.Component->GetComponentLocation() - Location).GetSafeNormal()) >= AngleCos)
		{
			Components.Emplace(Overlap.Component.Get());
		}
	}

	return !Components.IsEmpty();
}
