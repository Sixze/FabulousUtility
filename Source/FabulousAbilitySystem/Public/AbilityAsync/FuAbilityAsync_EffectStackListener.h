#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "AbilityTasks/FuAbilityTask_EffectStackListener.h"
#include "FuAbilityAsync_EffectStackListener.generated.h"

UCLASS(DisplayName = "Fu Effect Stack Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_EffectStackListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TSubclassOf<UGameplayEffect> EffectClass;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Effect Stack Listener Ability Async")
	FFuEffectStackListenerDelegate OnStackChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectStackListener* FuListenForEffectStackChangeOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Effect Class") TSubclassOf<UGameplayEffect> InEffectClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_EffectStackListener* FuListenForEffectStackChange(
		UAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Effect Class") TSubclassOf<UGameplayEffect> InEffectClass);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& EffectSpecification,
	                                               FActiveGameplayEffectHandle EffectHandle) const;

	void AbilitySystem_OnActiveGameplayEffectRemoved(const FActiveGameplayEffect& ActiveEffect) const;

	void AbilitySystem_OnEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewCount, int32 PreviousCount) const;
};
