#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_TagListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuTagChangedDelegate, const FGameplayTag&, Tag);

UCLASS(DisplayName = "Fu Tag Listener Ability Async")
class FABULOUSUTILITY_API UFuAbilityAsync_TagListener : public UAbilityAsync
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, DisplayName = "Tags", Meta = (AllowPrivateAccess))
	FGameplayTagContainer Tags1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Tag Listener Ability Async")
	FFuTagChangedDelegate OnTagAdded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|Fu Tag Listener Ability Async")
	FFuTagChangedDelegate OnTagRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* FuListenForTagChangeActor(const AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* FuListenForTagsChangeActor(const AActor* Actor, FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* FuListenForTagChange(UAbilitySystemComponent* AbilitySystem, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* FuListenForTagsChange(UAbilitySystemComponent* AbilitySystem, FGameplayTagContainer Tags);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void OnTagChanged(FGameplayTag Tag, int32 NewCount) const;
};
