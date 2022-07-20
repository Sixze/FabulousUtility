#pragma once

#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuEventDataUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuEventDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayEventData MakeEventDataFromEffectModificationData(const FGameplayEffectModCallbackData& Data);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility",
		Meta = (AutoCreateRefTerm = "InstigatorActorInfo, TargetActorInfo"))
	static FGameplayEventData MakeEventDataFromActorInfos(const FGameplayAbilityActorInfo& InstigatorActorInfo,
	                                                      const FGameplayAbilityActorInfo& TargetActorInfo);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility", Meta = (AutoCreateRefTerm = "InstigatorActorInfo"))
	static FGameplayEventData MakeEventDataFromActorInfoAndAvatar(const FGameplayAbilityActorInfo& InstigatorActorInfo,
	                                                              const AActor* TargetAvatar);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility")
	static FGameplayEventData MakeEventDataFromAbilitySystemAndAvatar(const UAbilitySystemComponent* InstigatorAbilitySystem,
	                                                                  const AActor* TargetAvatar);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Event Data Utility")
	static FGameplayEventData MakeEventDataFromAvatars(const AActor* InstigatorAvatar, const AActor* TargetAvatar);
};
