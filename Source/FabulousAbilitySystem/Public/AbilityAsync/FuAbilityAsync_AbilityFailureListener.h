#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_AbilityFailureListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFuAbilityFailureListenerDelegate, FGameplayAbilitySpecHandle, AbilityHandle,
                                             const FGameplayTagContainer&, FailureTags);

UCLASS(DisplayName = "Fu Ability Failure Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_AbilityFailureListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer AbilityTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer FailureTags;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Ability Failure Listener")
	FFuAbilityFailureListenerDelegate OnAbilityFailed;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* ListenForAbilityFailureOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Ability Tag") FGameplayTag InAbilityTag,
		UPARAM(DisplayName = "Failure Tags") FGameplayTagContainer InFailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* ListenForAbilitiesFailureOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Ability Tags") FGameplayTagContainer InAbilityTags,
		UPARAM(DisplayName = "Failure Tags") FGameplayTagContainer InFailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* ListenForAbilityFailure(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Ability Tag") FGameplayTag InAbilityTag,
		UPARAM(DisplayName = "Failure Tags") FGameplayTagContainer InFailureTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityFailureListener* ListenForAbilitiesFailure(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Ability Tags") FGameplayTagContainer InAbilityTags,
		UPARAM(DisplayName = "Failure Tags") FGameplayTagContainer InFailureTags);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnAbilityFailed(FGameplayAbilitySpecHandle AbilityHandle, UGameplayAbility* Ability,
	                                   const FGameplayTagContainer& ActivationFailureTags) const;
};
