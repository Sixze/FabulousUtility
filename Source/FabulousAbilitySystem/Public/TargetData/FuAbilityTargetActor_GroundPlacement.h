#pragma once

#include "Abilities/GameplayAbilityTargetActor.h"
#include "Engine/CollisionProfile.h"
#include "FuAbilityTargetActor_GroundPlacement.generated.h"

UCLASS(DisplayName = "Fu Ground Placement Ability Target Actor")
class FABULOUSABILITYSYSTEM_API AFuAbilityTargetActor_GroundPlacement : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float SphereSweepRadius{84.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2f SphereSweepHeight{-100.0f, 100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ClampMax = 90, ForceUnits = "deg"))
	float MaxSlopeAngle{20.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceUnits = "cm"))
	float TraceHorizontalOffset{200.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FCollisionProfileName TraceProfile{UCollisionProfile::Pawn_ProfileName};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FVector GroundPlaceLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FRotator GroundPlaceRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bGroundPlaceValid;

public:
	AFuAbilityTargetActor_GroundPlacement();

	virtual void Tick(float DeltaTime) override;

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual bool IsConfirmTargetingAllowed() override;

	virtual void ConfirmTargeting() override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	bool PerformGroundPlacement(FVector& ResultLocation, FRotator& ResultRotation) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Fu Ground Placement Ability Target Actor")
	void OnGroundPlaceValidChanged(bool bNewLastGroundPlaceValid);
};
