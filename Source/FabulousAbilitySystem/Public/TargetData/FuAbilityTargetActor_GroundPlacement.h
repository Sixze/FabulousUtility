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

	// Prevents placement on surfaces whose slope angle exceeds this value.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ClampMax = 90, ForceUnits = "deg"))
	float SlopeAngleThreshold{20.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings", AdvancedDisplay, Meta = (ClampMin = 0, ClampMax = 1))
	float SlopeAngleThresholdCos{FMath::Cos(FMath::DegreesToRadians(20.0f))};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceUnits = "cm"))
	float TraceHorizontalOffset{200.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FCollisionProfileName TraceProfile{UCollisionProfile::Pawn_ProfileName};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FVector FinalLocation{ForceInit};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FRotator FinalRotation{ForceInit};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	uint8 bFinalTransformValid : 1 {false};

public:
	AFuAbilityTargetActor_GroundPlacement();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif

	virtual void Tick(float DeltaTime) override;

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual bool IsConfirmTargetingAllowed() override;

	virtual void ConfirmTargeting() override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	bool PerformGroundPlacement(FVector& OutFinalLocation, FRotator& OutFinalRotation) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Fu Ground Placement Ability Target Actor")
	void OnFinalTransformValidChanged(bool bNewLastGroundPlaceValid);
};
