#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_AbilityGiveListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuAbilityGiveListenerDelegate, FGameplayAbilitySpecHandle, AbilityHandle);

UCLASS(DisplayName = "Fu Ability Give Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_AbilityGiveListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer AbilityTags;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Give Listener Ability Async")
	FFuAbilityGiveListenerDelegate OnAbilityGiven;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Ability Give Listener Ability Async")
	FFuAbilityGiveListenerDelegate OnAbilityRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityGiveListener* FuListenForAbilityGiveOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Ability Tag") FGameplayTag InAbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityGiveListener* FuListenForAbilitiesGiveOnActor(
		const AActor* Actor,
		UPARAM(DisplayName = "Ability Tags") FGameplayTagContainer InAbilityTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityGiveListener* FuListenForAbilityGive(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Ability Tag") FGameplayTag InAbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_AbilityGiveListener* FuListenForAbilitiesGive(
		UFuAbilitySystemComponent* AbilitySystem,
		UPARAM(DisplayName = "Ability Tags") FGameplayTagContainer InAbilityTags);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpecification) const;

	void AbilitySystem_OnAbilityRemoved(const FGameplayAbilitySpec& AbilitySpecification) const;
};
