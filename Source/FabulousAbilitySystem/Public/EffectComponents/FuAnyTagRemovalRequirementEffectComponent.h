#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectComponent.h"
#include "FuAnyTagRemovalRequirementEffectComponent.generated.h"

struct FGameplayEffectRemovalInfo;

// Prevents the application of the effect or removes it if the owner of this effect has ANY of the specified tags.
UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAnyTagRemovalRequirementEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	// Effect will be removed if the owner of this effect has ANY of these tags. Also prevents effect application.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FInheritedTagContainer RemovalRequirementTags;

public:
	UFuAnyTagRemovalRequirementEffectComponent();

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif

	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveEffects,
	                                    const FGameplayEffectSpec& EffectSpecification) const override;

	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveEffects,
	                                         FActiveGameplayEffect& ActiveEffect) const override;

	virtual void OnGameplayEffectChanged() const override;

private:
	void RefreshRemovalRequirementTags();

	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 TagCount, FActiveGameplayEffectHandle EffectHandle) const;

	void Effect_OnRemoved(const FGameplayEffectRemovalInfo& RemovalInfo, UAbilitySystemComponent* AbilitySystem,
	                      TArray<FDelegateHandle> TagChangedDelegateHandles) const;
};
