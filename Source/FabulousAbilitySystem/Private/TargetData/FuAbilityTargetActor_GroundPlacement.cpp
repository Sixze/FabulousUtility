#include "TargetData//FuAbilityTargetActor_GroundPlacement.h"

#include "AbilitySystemComponent.h"
#include "FuActorUtility.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/FuVector.h"
#include "TargetData/FuAbilityTargetData_LocationAndRotation.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTargetActor_GroundPlacement)

AFuAbilityTargetActor_GroundPlacement::AFuAbilityTargetActor_GroundPlacement()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

void AFuAbilityTargetActor_GroundPlacement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(SourceActor))
	{
		return;
	}

	const auto bNewGroundPlaceValid{PerformGroundPlacement(GroundPlaceLocation, GroundPlaceRotation)};

	SetActorLocationAndRotation(GroundPlaceLocation, GroundPlaceRotation);

	if (bGroundPlaceValid != bNewGroundPlaceValid)
	{
		bGroundPlaceValid = bNewGroundPlaceValid;

		OnGroundPlaceValidChanged(bNewGroundPlaceValid);
	}
}

void AFuAbilityTargetActor_GroundPlacement::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

bool AFuAbilityTargetActor_GroundPlacement::IsConfirmTargetingAllowed()
{
	return bGroundPlaceValid;
}

void AFuAbilityTargetActor_GroundPlacement::ConfirmTargeting()
{
	Super::ConfirmTargeting();

	if (IsPendingKillPending())
	{
		return;
	}

	const auto* ActorInfo{OwningAbility->GetCurrentActorInfo()};

	if (FU_ENSURE(ActorInfo->AbilitySystemComponent.IsValid()) && ActorInfo->IsLocallyControlled())
	{
		ActorInfo->AbilitySystemComponent->GenericLocalConfirmCallbacks.AddUniqueDynamic(this, &ThisClass::ConfirmTargeting);
	}
}

void AFuAbilityTargetActor_GroundPlacement::ConfirmTargetingAndContinue()
{
	if (IsConfirmTargetingAllowed())
	{
		auto* TargetData{new FFuAbilityTargetData_LocationAndRotation{}};

		TargetData->Location = GroundPlaceLocation;
		TargetData->Rotation = GroundPlaceRotation;

		TargetDataReadyDelegate.Broadcast({TargetData});
	}
}

bool AFuAbilityTargetActor_GroundPlacement::PerformGroundPlacement(FVector& ResultLocation, FRotator& ResultRotation) const
{
	const auto ActorFeetLocation{UFuActorUtility::GetActorFeetLocation(SourceActor)};
	const auto ActorForwardVector{SourceActor->GetActorForwardVector()};

	ResultLocation = ActorFeetLocation + ActorForwardVector * TraceHorizontalOffset;
	ResultRotation = {0.0f, UFuVector::DirectionToAngleXY(ActorForwardVector), 0.0f};

	const FVector TraceStart{
		ResultLocation.X, ResultLocation.Y,
		ActorFeetLocation.Z + SphereSweepHeight.GetMax() + UCharacterMovementComponent::MIN_FLOOR_DIST
	};
	const FVector TraceEnd{
		ResultLocation.X, ResultLocation.Y,
		ActorFeetLocation.Z + SphereSweepHeight.GetMin() - UCharacterMovementComponent::MAX_FLOOR_DIST
	};

	// Navigation trace.

	auto* NavigationSystem{FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())};
	if (IsValid(NavigationSystem))
	{
		auto* NavigationData{NavigationSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate)};
		FVector NavigationHitLocation;

		if (IsValid(NavigationData) && NavigationData->Raycast(TraceStart, TraceEnd, NavigationHitLocation, {}))
		{
			return false;
		}
	}

	// Sphere sweep.

	static const FName SphereSweepTagName{FString::Printf(TEXT("%hs (Sphere Sweep)"), __FUNCTION__)};

	FHitResult SphereSweepHit;
	GetWorld()->SweepSingleByProfile(SphereSweepHit, {TraceStart.X, TraceStart.Y, TraceStart.Z + SphereSweepRadius},
	                                 {TraceEnd.X, TraceEnd.Y, TraceEnd.Z + SphereSweepRadius}, FQuat::Identity,
	                                 TraceProfile.Name, FCollisionShape::MakeSphere(SphereSweepRadius),
	                                 {SphereSweepTagName, false, SourceActor});

	if (!SphereSweepHit.IsValidBlockingHit())
	{
		return false;
	}

	ResultLocation = SphereSweepHit.Location;
	ResultLocation.Z -= SphereSweepRadius;

	const auto SlopeVector{SphereSweepHit.Location - SphereSweepHit.ImpactPoint};
	const auto SlopeVectorLength{SlopeVector.Size()};
	const auto SlopeCos{(SlopeVector / SlopeVectorLength).Z};

	if (FMath::RadiansToDegrees(FMath::Acos(SlopeCos)) > MaxSlopeAngle + 1.0f)
	{
		return false;
	}

	// Slope trace.

	static const FName SlopeTraceTagName{FString::Printf(TEXT("%hs (Slope Trace)"), __FUNCTION__)};

	FHitResult SlopeTraceHit;
	GetWorld()->LineTraceSingleByProfile(SlopeTraceHit,
	                                     TraceStart,
	                                     {
		                                     SphereSweepHit.Location.X, SphereSweepHit.Location.Y, SphereSweepHit.Location.Z -
			                                     SlopeVectorLength / SlopeCos - UCharacterMovementComponent::MAX_FLOOR_DIST
	                                     },
	                                     TraceProfile.Name, {SlopeTraceTagName, false, SourceActor});

	if (!SlopeTraceHit.IsValidBlockingHit())
	{
		return false;
	}

	// Visibility trace.

	static const FName VisibilityTraceTagName{FString::Printf(TEXT("%hs (Visibility Trace)"), __FUNCTION__)};

	FHitResult VisibilityTraceHit;
	GetWorld()->LineTraceSingleByProfile(VisibilityTraceHit,
	                                     SourceActor->GetActorLocation(),
	                                     {
		                                     SphereSweepHit.Location.X, SphereSweepHit.Location.Y, SphereSweepHit.Location.Z -
			                                     SphereSweepRadius + UCharacterMovementComponent::MAX_FLOOR_DIST
	                                     },
	                                     TraceProfile.Name, {VisibilityTraceTagName, false, SourceActor});

	return !VisibilityTraceHit.IsValidBlockingHit();
}

void AFuAbilityTargetActor_GroundPlacement::OnGroundPlaceValidChanged_Implementation(bool bNewLastGroundPlaceValid) {}
