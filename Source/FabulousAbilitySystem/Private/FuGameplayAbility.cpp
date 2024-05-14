#include "FuGameplayAbility.h"

#include "AbilitySystemGlobals.h"
#include "FuAbilitySystemComponent.h"
#include "Utility/FuAbilityUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayAbility)

UFuGameplayAbility::UFuGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// https://github.com/tranek/GASDocumentation#concepts-ga-definition-remotecancel
	bServerRespectsRemoteAbilityCancellation = false;

	static const auto CheckCostBlueprintFunctionName{GET_FUNCTION_NAME_CHECKED(ThisClass, CheckCostBlueprint)};
	bCheckCostBlueprintImplemented = GetClass()->IsFunctionImplementedInScript(CheckCostBlueprintFunctionName);
}

void UFuGameplayAbility::SetShouldBlockOtherAbilities(const bool bShouldBlockAbilities)
{
	if (!bIsActive || GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced ||
	    bShouldBlockAbilities == bIsBlockingOtherAbilities)
	{
		return;
	}

	Super::SetShouldBlockOtherAbilities(bShouldBlockAbilities);

	auto* FuAbilitySystem{Cast<UFuAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent)};
	if (!IsValid(FuAbilitySystem))
	{
		return;
	}

	if (bIsBlockingOtherAbilities)
	{
		FuAbilitySystem->BlockAbilitiesWithoutAllTags(BlockAbilitiesWithoutAllTags);
	}
	else
	{
		FuAbilitySystem->UnBlockAbilitiesWithoutAllTags(BlockAbilitiesWithoutAllTags);
	}
}

bool UFuGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle AbilityHandle,
                                   const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* FailureTags) const
{
	if (!Super::CheckCost(AbilityHandle, ActorInfo, FailureTags))
	{
		return false;
	}

	if (bCheckCostBlueprintImplemented && !CheckCostBlueprint(*ActorInfo, AbilityHandle))
	{
		const auto& CostFailureTag{UAbilitySystemGlobals::Get().ActivateFailCostTag};
		if (FailureTags != nullptr && CostFailureTag.IsValid())
		{
			FailureTags->AddTag(CostFailureTag);
		}

		return false;
	}

	return true;
}

void UFuGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(AbilityHandle, ActorInfo, ActivationInfo);

	ApplyCostBlueprint(*ActorInfo, AbilityHandle, ActivationInfo);
}

void UFuGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& AbilitySpecification)
{
	Super::OnAvatarSet(ActorInfo, AbilitySpecification);

	if (bAutoActivateWhenGiven)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(AbilitySpecification.Handle, false);
	}
}

void UFuGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(AbilityHandle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	auto* FuAbilitySystem{Cast<UFuAbilitySystemComponent>(ActorInfo->AbilitySystemComponent)};

	if (bIsBlockingOtherAbilities && FU_ENSURE(IsValid(FuAbilitySystem)))
	{
		FuAbilitySystem->BlockAbilitiesWithoutAllTags(BlockAbilitiesWithoutAllTags);
	}

	if (CancelAbilitiesWithoutAllTags.IsValid())
	{
		ActorInfo->AbilitySystemComponent->CancelAbilities(nullptr, &CancelAbilitiesWithoutAllTags, this);
	}

	if (RemoveAbilitiesWithAnyTags.IsValid())
	{
		UFuAbilityUtility::RemoveAbilitiesWithAnyTags(ActorInfo->AbilitySystemComponent.Get(), RemoveAbilitiesWithAnyTags, AbilityHandle);
	}
}

void UFuGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const bool bReplicateEndAbility, const bool bCanceled)
{
	if (IsEndAbilityValid(AbilityHandle, ActorInfo) &&
	    (bIsActive || GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced))
	{
		auto* FuAbilitySystem{Cast<UFuAbilitySystemComponent>(ActorInfo->AbilitySystemComponent)};

		if (FU_ENSURE(IsValid(FuAbilitySystem)) && IsBlockingOtherAbilities())
		{
			FuAbilitySystem->UnBlockAbilitiesWithoutAllTags(BlockAbilitiesWithoutAllTags);
		}
	}

	Super::EndAbility(AbilityHandle, ActorInfo, ActivationInfo, bReplicateEndAbility, bCanceled);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
bool UFuGameplayAbility::BatchRpcActivateAbility(const FGameplayAbilitySpecHandle AbilityHandle, const bool bEndAbilityImmediately)
{
	return UFuAbilityUtility::BatchRpcActivateAbility(GetAbilitySystemComponentFromActorInfo(), AbilityHandle, bEndAbilityImmediately);
}

void UFuGameplayAbility::BatchRpcEndAbility()
{
	K2_EndAbility();
}
