#pragma once

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuAbilitySystemUtility.generated.h"

UCLASS()
class FABULOUSABILITYSYSTEM_API UFuAbilitySystemUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability System Utility")
	static UAbilitySystemComponent* GetAbilitySystem(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability System Utility", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool TryGetAbilitySystem(const UObject* Object, UAbilitySystemComponent*& AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability System Utility", Meta = (ReturnDisplayName = "Actor Info"))
	static const FGameplayAbilityActorInfo& GetActorInfo(const UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability System Utility", Meta = (ReturnDisplayName = "Avatar"))
	static AActor* GetAvatar(const UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Ability System|Ability System Utility",
		Meta = (AutoCreateRefTerm = "ParentTag", ReturnDisplayName = "Descendant Tag"))
	static FGameplayTag FindFirstOwnedDescendantTag(const UAbilitySystemComponent* AbilitySystem, const FGameplayTag& ParentTag);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability System Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Ability Handle"))
	static FGameplayAbilitySpecHandle GiveAbilityWithDynamicTags(UAbilitySystemComponent* AbilitySystem,
	                                                             TSubclassOf<UGameplayAbility> AbilityClass,
	                                                             int32 Level, const FGameplayTagContainer& Tags,
	                                                             UObject* SourceObject = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Utility",
		Meta = (AutoCreateRefTerm = "SetByCallerMagnitudes", ReturnDisplayName = "Effect Handle"))
	static FActiveGameplayEffectHandle ApplyEffectWithSetByCallerMagnitudes(UAbilitySystemComponent* AbilitySystem,
	                                                                        TSubclassOf<UGameplayEffect> EffectClass,
	                                                                        const TMap<FGameplayTag, float>& SetByCallerMagnitudes);

	static FActiveGameplayEffectHandle ApplyEffectWithSetByCallerMagnitudes(UAbilitySystemComponent* AbilitySystem,
	                                                                        TSubclassOf<UGameplayEffect> EffectClass,
	                                                                        TMap<FGameplayTag, float>&& SetByCallerMagnitudes);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Success"))
	static bool AddLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, bool bReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Utility",
		Meta = (AutoCreateRefTerm = "Tag", ReturnDisplayName = "Success"))
	static bool RemoveLooseTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag, bool bReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Success"))
	static bool AddLooseTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags, bool bReplicate = false);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Ability System|Ability Utility",
		Meta = (AutoCreateRefTerm = "Tags", ReturnDisplayName = "Success"))
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
