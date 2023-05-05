#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "FuEventDataUtility.generated.h"

struct FGameplayEffectModCallbackData;
struct FGameplayAbilityActorInfo;
class UAbilitySystemComponent;

UCLASS()
class FABULOUSUTILITY_API UFuEventDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayEventData MakeEventDataFromEffectModificationData(const FGameplayEffectModCallbackData& Data);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility", Meta = (ReturnDisplayName = "Event Data"))
	static FGameplayEventData MakeEventDataFromAbilitySystems(const UAbilitySystemComponent* InstigatorAbilitySystem,
	                                                          const UAbilitySystemComponent* TargetAbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility", Meta = (ReturnDisplayName = "Event Data"))
	static FGameplayEventData MakeEventDataFromAbilitySystemAndAvatar(const UAbilitySystemComponent* InstigatorAbilitySystem,
	                                                                  const AActor* TargetAvatar);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility", Meta = (ReturnDisplayName = "Event Data"))
	static FGameplayEventData MakeEventDataFromAvatarAndAbilitySystem(const AActor* InstigatorAvatar,
	                                                                  const UAbilitySystemComponent* TargetAbilitySystem);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility",
		Meta = (AutoCreateRefTerm = "InstigatorActorInfo, TargetActorInfo", ReturnDisplayName = "Event Data"))
	static FGameplayEventData MakeEventDataFromActorInfos(const FGameplayAbilityActorInfo& InstigatorActorInfo,
	                                                      const FGameplayAbilityActorInfo& TargetActorInfo);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility",
		Meta = (AutoCreateRefTerm = "InstigatorActorInfo", ReturnDisplayName = "Event Data"))
	static FGameplayEventData MakeEventDataFromActorInfoAndAvatar(const FGameplayAbilityActorInfo& InstigatorActorInfo,
	                                                              const AActor* TargetAvatar);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility", Meta = (ReturnDisplayName = "Event Data"))
	static FGameplayEventData MakeEventDataFromAvatars(const AActor* InstigatorAvatar, const AActor* TargetAvatar);
};
