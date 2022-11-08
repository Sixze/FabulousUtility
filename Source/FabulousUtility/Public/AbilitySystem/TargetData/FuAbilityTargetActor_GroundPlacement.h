#pragma once

#include "Abilities/GameplayAbilityTargetActor.h"
#include "FuAbilityTargetActor_GroundPlacement.generated.h"

UCLASS(DisplayName = "Fu Ground Placement Ability Target Actor")
class FABULOUSUTILITY_API AFuAbilityTargetActor_GroundPlacement : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float SphereSweepRadius{84.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	FVector2D SphereSweepHeight{-100.0f, 100.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ClampMax = 90, ForceUnits = "deg"))
	float MaxSlopeAngle{20.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", Meta = (ForceUnits = "cm"))
	float TraceHorizontalOffset{200.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	FCollisionProfileName TraceProfile{UCollisionProfile::Pawn_ProfileName};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FVector LastGroundPlaceLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FRotator LastGroundPlaceRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	bool bLastGroundPlaceValid;

public:
	AFuAbilityTargetActor_GroundPlacement();

	virtual void Tick(float DeltaTime) override;

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual bool IsConfirmTargetingAllowed() override;

	virtual void ConfirmTargeting() override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	bool PerformGroundPlacement(FVector& ResultLocation, FRotator& ResultRotation) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Fu Ground Placement Ability Target Actor",
		DisplayName = "Last Ground Place Valid Changed")
	void ReceiveLastGroundPlaceValidChanged(bool bNewLastGroundPlaceValid);
};
