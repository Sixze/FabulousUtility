#pragma once

#include "GameplayEffect.h"
#include "FuGameplayEffect.generated.h"

UCLASS(AutoExpandCategories = ("Settings|Fu Gameplay Effect", "State|Fu Gameplay Effect"))
class FABULOUSABILITYSYSTEM_API UFuGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

protected:
	// Effect will be removed if the actor has ANY of these tags. Also prevents effect application.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Fu Gameplay Effect")
	FGameplayTagContainer RemovalRequirementAnyTag;

public:
	const FGameplayTagContainer& GetRemovalRequirementAnyTag() const;
};

inline const FGameplayTagContainer& UFuGameplayEffect::GetRemovalRequirementAnyTag() const
{
	return RemovalRequirementAnyTag;
}
