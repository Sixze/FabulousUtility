#pragma once

#include "Abilities/Async/AbilityAsync.h"
#include "FuAbilityAsync_TagListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuTagListenerDelegate, const FGameplayTag&, Tag);

UCLASS(DisplayName = "Fu Tag Listener Ability Async")
class FABULOUSABILITYSYSTEM_API UFuAbilityAsync_TagListener : public UAbilityAsync
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FGameplayTagContainer Tags;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Tag Listener")
	FFuTagListenerDelegate OnTagAdded;

	UPROPERTY(BlueprintAssignable, Category = "Fabulous Ability System|Ability Async|Tag Listener")
	FFuTagListenerDelegate OnTagRemoved;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* ListenForTagChangeOnActor(const AActor* Actor,
	                                                              UPARAM(DisplayName = "Tag") FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* ListenForTagsChangeOnActor(const AActor* Actor,
	                                                               UPARAM(DisplayName = "Tags") FGameplayTagContainer InTags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* ListenForTagChange(UAbilitySystemComponent* AbilitySystem,
	                                                       UPARAM(DisplayName = "Tag") FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Async", BlueprintInternalUseOnly)
	static UFuAbilityAsync_TagListener* ListenForTagsChange(UAbilitySystemComponent* AbilitySystem,
	                                                        UPARAM(DisplayName = "Tags") FGameplayTagContainer InTags);

public:
	virtual void Activate() override;

	virtual void EndAction() override;

private:
	void AbilitySystem_OnTagChanged(FGameplayTag Tag, int32 TagCount) const;
};
