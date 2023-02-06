#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilitySystemUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuAbilitySystemUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability System Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetAbilitySystem(const UObject* Object, UAbilitySystemComponent*& AbilitySystem, bool bAllowFindComponent = true);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System Utility")
	static const FGameplayAbilityActorInfo& GetActorInfo(const UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System Utility")
	static AActor* GetAvatar(const UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Ability System Utility", Meta = (AutoCreateRefTerm = "ParentTag"))
	static FGameplayTag GetFirstOwnedDescendantTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& ParentTag);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Fabulous Utility|Fu Ability System Utility",
		Meta = (AutoCreateRefTerm = "DynamicAbilityTags"))
	static FGameplayAbilitySpecHandle GiveAbilityWithDynamicTags(UAbilitySystemComponent* AbilitySystem,
	                                                             TSubclassOf<UGameplayAbility> AbilityClass,
	                                                             int32 Level, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tag"))
	static bool AddLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, bool bReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tag"))
	static bool RemoveLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, bool bReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tags"))
	static bool AddLooseTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags, bool bReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Ability Utility", Meta = (AutoCreateRefTerm = "Tags"))
	static bool RemoveLooseTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags, bool bReplicate = false);
};

inline const FGameplayAbilityActorInfo& UFuAbilitySystemUtility::GetActorInfo(const UAbilitySystemComponent* AbilitySystem)
{
	static const FGameplayAbilityActorInfo None;

	return FU_ENSURE(IsValid(AbilitySystem)) ? *AbilitySystem->AbilityActorInfo : None;
}

inline AActor* UFuAbilitySystemUtility::GetAvatar(const UAbilitySystemComponent* AbilitySystem)
{
	return FU_ENSURE(IsValid(AbilitySystem)) ? AbilitySystem->GetAvatarActor_Direct() : nullptr;
}
