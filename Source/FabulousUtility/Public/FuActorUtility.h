#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "FuActorUtility.generated.h"

class UActorComponent;

UCLASS()
class FABULOUSUTILITY_API UFuActorUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Actor Utility", Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Value"))
	static bool IsActorLoadedFromLevel(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Actor Utility",
		DisplayName = "Is Actor Loaded from Level (Expanded)", Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool IsActorLoadedFromLevelExpanded(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Actor Utility",
		Meta = (DeterminesOutputType = "ComponentClass", DynamicOutputParam = "Component", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryFindComponentByClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& Component);

	// Not the same as AActor::GetActorEyesViewPoint(), since this function
	// returns the exact location and rotation of the camera, if possible.
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Actor Utility")
	static void GetPlayerViewPoint(const AActor* Actor, FVector& Location, FRotator& Rotation);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Actor Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Feet Location"))
	static FVector GetActorFeetLocation(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Actor Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Feet Offset"))
	static FVector GetActorFeetOffset(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Actor Utility",
		Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "Location, Rotation", ReturnDisplayName = "Value"))
	static bool IsEncroachingBlockingGeometry(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                          const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Actor Utility", DisplayName = "Is Encroaching Blocking Geometry (Expanded)",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Location, Rotation"))
	static bool IsEncroachingBlockingGeometryExpanded(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                                  const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Actor Utility",
		Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "Location, Rotation", ReturnDisplayName = "Value"))
	static bool IsEncroachingBlockingGeometryWithAdjustment(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                                        const FVector& Location, const FRotator& Rotation,
	                                                        FVector& ProposedAdjustment);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Actor Utility",
		DisplayName = "Is Encroaching Blocking Geometry with Adjustment (Expanded)",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Location, Rotation"))
	static bool IsEncroachingBlockingGeometryWithAdjustmentExpanded(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                                                const FVector& Location, const FRotator& Rotation,
	                                                                FVector& ProposedAdjustment);
};

inline bool UFuActorUtility::IsActorLoadedFromLevelExpanded(const AActor* Actor)
{
	return IsActorLoadedFromLevel(Actor);
}

inline bool UFuActorUtility::IsEncroachingBlockingGeometryExpanded(const UObject* WorldContext, const TSubclassOf<AActor> ActorClass,
                                                                   const FVector& Location, const FRotator& Rotation)
{
	return IsEncroachingBlockingGeometry(WorldContext, ActorClass, Location, Rotation);
}

inline bool UFuActorUtility::IsEncroachingBlockingGeometryWithAdjustmentExpanded(const UObject* WorldContext,
                                                                                 const TSubclassOf<AActor> ActorClass,
                                                                                 const FVector& Location, const FRotator& Rotation,
                                                                                 FVector& ProposedAdjustment)
{
	return IsEncroachingBlockingGeometryWithAdjustment(WorldContext, ActorClass, Location, Rotation, ProposedAdjustment);
}
