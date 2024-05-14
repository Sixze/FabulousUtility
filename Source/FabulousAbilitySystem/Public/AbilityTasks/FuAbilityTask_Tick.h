#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FuAbilityTask_Tick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuTickDelegate, float, DeltaTime);

UCLASS(DisplayName = "Fu Tick Ability Task")
class FABULOUSABILITYSYSTEM_API UFuAbilityTask_Tick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Tasks|Tick")
	FFuTickDelegate OnTick;

public:
	UFuAbilityTask_Tick();

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Tasks",
		BlueprintInternalUseOnly, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UFuAbilityTask_Tick* WaitForTick(UGameplayAbility* OwningAbility);

protected:
	virtual void TickTask(float DeltaTime) override;
};
