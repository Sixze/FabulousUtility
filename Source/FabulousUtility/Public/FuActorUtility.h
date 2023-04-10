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
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility", Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Value"))
	static bool IsActorLoadedFromLevel(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchIsActorLoadedFromLevel(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (DeterminesOutputType = "ComponentClass", DynamicOutputParam = "Component", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryFindComponentByClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& Component);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Feet Location"))
	static FVector GetActorFeetLocation(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (DefaultToSelf = "Actor", ReturnDisplayName = "Feet Offset"))
	static FVector GetActorFeetOffset(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "Location, Rotation", ReturnDisplayName = "Value"))
	static bool IsEncroachingBlockingGeometry(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                          const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Location, Rotation"))
	static bool SwitchIsEncroachingBlockingGeometry(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                                const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "Location, Rotation", ReturnDisplayName = "Value"))
	static bool IsEncroachingBlockingGeometryWithAdjustment(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                                        const FVector& Location, const FRotator& Rotation,
	                                                        FVector& ProposedAdjustment);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (WorldContext = "WorldContext", ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "Location, Rotation"))
	static bool SwitchIsEncroachingBlockingGeometryWithAdjustment(const UObject* WorldContext, TSubclassOf<AActor> ActorClass,
	                                                              const FVector& Location, const FRotator& Rotation,
	                                                              FVector& ProposedAdjustment);
};

inline bool UFuActorUtility::SwitchIsActorLoadedFromLevel(const AActor* Actor)
{
	return IsActorLoadedFromLevel(Actor);
}

inline bool UFuActorUtility::SwitchIsEncroachingBlockingGeometry(const UObject* WorldContext, const TSubclassOf<AActor> ActorClass,
                                                                 const FVector& Location, const FRotator& Rotation)
{
	return IsEncroachingBlockingGeometry(WorldContext, ActorClass, Location, Rotation);
}

inline bool UFuActorUtility::SwitchIsEncroachingBlockingGeometryWithAdjustment(const UObject* WorldContext,
                                                                               const TSubclassOf<AActor> ActorClass,
                                                                               const FVector& Location, const FRotator& Rotation,
                                                                               FVector& ProposedAdjustment)
{
	return IsEncroachingBlockingGeometryWithAdjustment(WorldContext, ActorClass, Location, Rotation, ProposedAdjustment);
}
