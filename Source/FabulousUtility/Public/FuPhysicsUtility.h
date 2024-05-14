#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuPhysicsUtility.generated.h"

enum ECollisionChannel : int;
struct FCollisionProfileName;
struct FOverlapResult;
struct FCollisionResponseParams;

UCLASS()
class FABULOUSUTILITY_API UFuPhysicsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void FindReachableActorsInRadius(const UObject* WorldContext, const FVector& Location,
	                                        float Radius, const FCollisionProfileName& CollisionProfile,
	                                        TFunctionRef<bool(const FOverlapResult& Overlap)> FilterPredicate,
	                                        TMap<AActor*, TArray<FHitResult>>& ReachableActors, const AActor* IgnoredActor = nullptr);

	static void FindReachableActorsInRadius(const UObject* WorldContext, const FVector& Location, float Radius,
	                                        ECollisionChannel CollisionChannel, const FCollisionResponseParams& CollisionResponses,
	                                        TFunctionRef<bool(const FOverlapResult& Overlap)> FilterPredicate,
	                                        TMap<AActor*, TArray<FHitResult>>& ReachableActors, const AActor* IgnoredActor = nullptr);

	static bool IsComponentReachableFromLocation(UPrimitiveComponent* Component, const FVector& Location,
	                                             ECollisionChannel CollisionChannel, const FCollisionResponseParams& CollisionResponses,
	                                             FHitResult& Hit, const AActor* IgnoredActor = nullptr);

	static bool IsComponentReachableByLineTrace(UPrimitiveComponent* Component, const FVector& TraceStart,
	                                            const FVector& TraceEnd, ECollisionChannel CollisionChannel,
	                                            const FCollisionResponseParams& CollisionResponses,
	                                            FHitResult& Hit, const AActor* IgnoredActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Physics Utility",
		Meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = "TraceStart, TraceEnd"))
	static bool LineTraceBone(UPrimitiveComponent* Primitive, FName BoneName,
	                          const FVector& TraceStart, const FVector& TraceEnd, FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Physics Utility", Meta = (WorldContext = "WorldContext",
		AutoCreateRefTerm = "Location, Rotation, Extent, IgnoreActors", ReturnDisplayName = "Success"))
	static bool BoxOverlapActors(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
	                             const FVector& Extent, const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
	                             const TArray<AActor*>& IgnoreActors, TArray<AActor*>& Actors);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Physics Utility", Meta = (WorldContext = "WorldContext",
		AutoCreateRefTerm = "Location, Rotation, Extent, IgnoreActors", ReturnDisplayName = "Success"))
	static bool BoxOverlapComponents(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
	                                 const FVector& Extent, const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
	                                 const TArray<AActor*>& IgnoreActors, TArray<UPrimitiveComponent*>& Components);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Physics Utility",
		Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "Location, Rotation, IgnoreActors", ReturnDisplayName = "Success"))
	static bool ConeOverlapActorsSimple(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
	                                    float Radius, float Angle, const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
	                                    const TArray<AActor*>& IgnoreActors, TArray<AActor*>& Actors);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Physics Utility",
		Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "Location, Rotation, IgnoreActors", ReturnDisplayName = "Success"))
	static bool ConeOverlapComponentsSimple(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
	                                        float Radius, float Angle, const TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannels,
	                                        const TArray<AActor*>& IgnoreActors, TArray<UPrimitiveComponent*>& Components);
};
