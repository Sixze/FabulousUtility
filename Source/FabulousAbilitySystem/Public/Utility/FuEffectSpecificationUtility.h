#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEffectSpecificationUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuEffectSpecificationUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Effect Specification Utility",
		Meta = (AutoCreateRefTerm = "SetByCallerTag", GameplayTagFilter = "SetByCaller", ReturnDisplayName = "Magnitude"))
	static float GetSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectHandle, const FGameplayTag& SetByCallerTag);
};
