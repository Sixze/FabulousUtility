#pragma once

#include "Components/ActorComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuActorUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuActorUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Actor Utility",
		Meta = (DeterminesOutputType = "ComponentClass", DynamicOutputParam = "Component", ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetComponentByClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& Component);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility", Meta = (DefaultToSelf = "Actor"))
	static FVector GetActorFeetLocation(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Actor Utility", Meta = (DefaultToSelf = "Actor"))
	static FVector GetActorFeetOffset(const AActor* Actor);
};
