#include "AbilitySystem/FuGameplayAbility.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/FuAbilitySystemComponent.h"
#include "AbilitySystem/Utility/FuAbilityUtility.h"

UFuGameplayAbility::UFuGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// https://github.com/tranek/GASDocumentation#concepts-ga-definition-remotecancel
	bServerRespectsRemoteAbilityCancellation = false;
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
		FuAbilitySystem->BlockAbilitiesWithoutTags(BlockAbilitiesWithoutTag);
	}
	else
	{
		FuAbilitySystem->UnBlockAbilitiesWithoutTags(BlockAbilitiesWithoutTag);
	}
}

bool UFuGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
                                   FGameplayTagContainer* FailureTags) const
{
	if (!CheckCostBlueprint(*ActorInfo, AbilityHandle))
	{
		const auto& CostFailureTag{UAbilitySystemGlobals::Get().ActivateFailCostTag};
		if (FailureTags && CostFailureTag.IsValid())
		{
			FailureTags->AddTag(CostFailureTag);
		}

		return false;
	}

	return Super::CheckCost(AbilityHandle, ActorInfo, FailureTags);
}

void UFuGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo) const
{
	ApplyCostBlueprint(*ActorInfo, AbilityHandle, ActivationInfo);

	Super::ApplyCost(AbilityHandle, ActorInfo, ActivationInfo);
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
		FuAbilitySystem->BlockAbilitiesWithoutTags(BlockAbilitiesWithoutTag);
	}

	if (CancelAbilitiesWithoutTag.IsValid())
	{
		ActorInfo->AbilitySystemComponent->CancelAbilities(nullptr, &CancelAbilitiesWithoutTag, this);
	}

	if (RemoveAbilitiesWithTag.IsValid())
	{
		UFuAbilityUtility::RemoveAbilitiesWithAnyTags(ActorInfo->AbilitySystemComponent.Get(), RemoveAbilitiesWithTag, AbilityHandle);
	}
}

void UFuGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle AbilityHandle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility,
                                    const bool bCanceled)
{
	if (IsEndAbilityValid(AbilityHandle, ActorInfo) &&
	    (bIsActive || GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced))
	{
		auto* FuAbilitySystem{Cast<UFuAbilitySystemComponent>(ActorInfo->AbilitySystemComponent)};

		if (FU_ENSURE(IsValid(FuAbilitySystem)) && IsBlockingOtherAbilities())
		{
			FuAbilitySystem->UnBlockAbilitiesWithoutTags(BlockAbilitiesWithoutTag);
		}
	}

	Super::EndAbility(AbilityHandle, ActorInfo, ActivationInfo, bReplicateEndAbility, bCanceled);
}

bool UFuGameplayAbility::CheckCostBlueprint_Implementation(const FGameplayAbilityActorInfo& ActorInfo,
                                                           const FGameplayAbilitySpecHandle AbilityHandle) const
{
	return true;
}

void UFuGameplayAbility::ApplyCostBlueprint_Implementation(const FGameplayAbilityActorInfo& ActorInfo,
                                                           const FGameplayAbilitySpecHandle AbilityHandle,
                                                           const FGameplayAbilityActivationInfo ActivationInfo) const {}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
bool UFuGameplayAbility::TryBatchRpcActivateAbility(const FGameplayAbilitySpecHandle AbilityHandle, const bool bEndAbilityImmediately)
{
	return UFuAbilityUtility::TryBatchRpcActivateAbility(GetAbilitySystemComponentFromActorInfo(), AbilityHandle, bEndAbilityImmediately);
}

void UFuGameplayAbility::BatchRpcEndAbility()
{
	K2_EndAbility();
}
