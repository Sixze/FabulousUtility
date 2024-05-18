#include "TargetData//FuAbilityTargetActor_GroundPlacement.h"

#include "AbilitySystemComponent.h"
#include "FuActorUtility.h"
#include "FuMacros.h"
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

#if WITH_EDITOR
void AFuAbilityTargetActor_GroundPlacement::PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent)
{
	if (ChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, SlopeAngleThreshold))
	{
		SlopeAngleThresholdCos = FMath::Cos(FMath::DegreesToRadians(SlopeAngleThreshold));
	}

	Super::PostEditChangeProperty(ChangedEvent);
}
#endif

void AFuAbilityTargetActor_GroundPlacement::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(SourceActor))
	{
		return;
	}

	const auto bNewFinalTransformValid{PerformGroundPlacement(FinalLocation, FinalRotation)};

	SetActorLocationAndRotation(FinalLocation, FinalRotation);

	if (bFinalTransformValid != bNewFinalTransformValid)
	{
		bFinalTransformValid = bNewFinalTransformValid;

		OnFinalTransformValidChanged(bNewFinalTransformValid);
	}
}

void AFuAbilityTargetActor_GroundPlacement::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

bool AFuAbilityTargetActor_GroundPlacement::IsConfirmTargetingAllowed()
{
	return bFinalTransformValid;
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
		// ReSharper disable once CppBoundToDelegateMethodIsNotMarkedAsUFunction
		ActorInfo->AbilitySystemComponent->GenericLocalConfirmCallbacks.AddUniqueDynamic(this, &ThisClass::ConfirmTargeting);
	}
}

void AFuAbilityTargetActor_GroundPlacement::ConfirmTargetingAndContinue()
{
	if (IsConfirmTargetingAllowed())
	{
		auto* TargetData{new FFuAbilityTargetData_LocationAndRotation{}};

		TargetData->Location = FinalLocation;
		TargetData->Rotation = FinalRotation;

		TargetDataReadyDelegate.Broadcast({TargetData});
	}
}

bool AFuAbilityTargetActor_GroundPlacement::PerformGroundPlacement(FVector& OutFinalLocation, FRotator& OutFinalRotation) const
{
	const auto ActorFeetLocation{UFuActorUtility::GetActorFeetLocation(SourceActor)};
	const auto ActorForwardDirection{SourceActor->GetActorForwardVector()};

	OutFinalLocation = ActorFeetLocation + ActorForwardDirection * TraceHorizontalOffset;
	OutFinalRotation = {0.0f, UFuVector::DirectionToAngleXY(ActorForwardDirection), 0.0f};

	const FVector TraceStart{
		OutFinalLocation.X, OutFinalLocation.Y,
		ActorFeetLocation.Z + SphereSweepHeight.GetMax() + UCharacterMovementComponent::MIN_FLOOR_DIST
	};
	const FVector TraceEnd{
		OutFinalLocation.X, OutFinalLocation.Y,
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

	OutFinalLocation = SphereSweepHit.Location;
	OutFinalLocation.Z -= SphereSweepRadius;

	const FVector3f SlopeVector{SphereSweepHit.Location - SphereSweepHit.ImpactPoint};
	const auto SlopeVectorLength{SlopeVector.Size()};
	const auto SlopeAngleCos{SlopeVector.Z / SlopeVectorLength};

	if (SlopeAngleCos < SlopeAngleThresholdCos)
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
			                                     SlopeVectorLength / SlopeAngleCos - UCharacterMovementComponent::MAX_FLOOR_DIST
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

void AFuAbilityTargetActor_GroundPlacement::OnFinalTransformValidChanged_Implementation(bool bNewLastGroundPlaceValid) {}
