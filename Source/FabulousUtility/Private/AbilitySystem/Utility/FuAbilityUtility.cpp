#include "AbilitySystem/Utility/FuAbilityUtility.h"

#include "AbilitySystem/FuGameplayAbility.h"

bool UFuAbilityUtility::CanActivateAbilityByTag(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& Tag)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	for (const auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.Ability->AbilityTags.HasTag(Tag) &&
		    AbilitySpecification.Ability->CanActivateAbility(AbilitySpecification.Handle, AbilitySystem->AbilityActorInfo.Get()))
		{
			return true;
		}
	}

	return false;
}

bool UFuAbilityUtility::CanActivateAbilityByClass(UAbilitySystemComponent* AbilitySystem,
                                                  const TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(AbilityClass)))
	{
		return false;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromClass(AbilityClass)};

	return AbilitySpecification != nullptr &&
	       AbilitySpecification->Ability->CanActivateAbility(AbilitySpecification->Handle, AbilitySystem->AbilityActorInfo.Get());
}

bool UFuAbilityUtility::CanActivateAbilityByHandle(UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayAbilitySpecHandle AbilityHandle)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()))
	{
		return false;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	return AbilitySpecification != nullptr &&
	       AbilitySpecification->Ability->CanActivateAbility(AbilityHandle, AbilitySystem->AbilityActorInfo.Get());
}

bool UFuAbilityUtility::TryBatchRpcActivateAbility(UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle AbilityHandle,
                                                   const bool bEndAbilityImmediately)
{
	// https://github.com/tranek/GASDocumentation#concepts-ga-batching

	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(AbilityHandle.IsValid()))
	{
		return false;
	}

	// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
	FScopedServerAbilityRPCBatcher RpcBatcher{AbilitySystem, AbilityHandle};

	const auto bAbilityActivated{AbilitySystem->TryActivateAbility(AbilityHandle, true)};

	if (!bEndAbilityImmediately)
	{
		return bAbilityActivated;
	}

	const auto* AbilitySpecification{AbilitySystem->FindAbilitySpecFromHandle(AbilityHandle)};

	if (AbilitySpecification == nullptr || !AbilitySpecification->IsActive() ||
	    !FU_ENSURE(AbilitySpecification->Ability->IsA<UFuGameplayAbility>()))
	{
		return bAbilityActivated;
	}

	// auto* Ability{Cast<UFuGameplayAbility>(AbilitySpecification->GetPrimaryInstance())};
	// Ability->BatchRpcEndAbility();

	if (AbilitySpecification->Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Cast<UFuGameplayAbility>(AbilitySpecification->Ability)->BatchRpcEndAbility();
	}
	else
	{
		for (auto* Ability : AbilitySpecification->GetAbilityInstances())
		{
			if (IsValid(Ability))
			{
				Cast<UFuGameplayAbility>(Ability)->BatchRpcEndAbility();
			}
		}
	}

	return bAbilityActivated;
}

void UFuAbilityUtility::RemoveAbilitiesWithAnyTags(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& Tags,
                                                   const FGameplayAbilitySpecHandle IgnoreAbilityHandle)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tags.IsValid()) || !FU_ENSURE(AbilitySystem->IsOwnerActorAuthoritative()))
	{
		return;
	}

	for (auto& AbilitySpecification : AbilitySystem->GetActivatableAbilities())
	{
		if (AbilitySpecification.Handle != IgnoreAbilityHandle &&
		    AbilitySpecification.Ability->AbilityTags.HasAny(Tags))
		{
			AbilitySystem->CancelAbilityHandle(AbilitySpecification.Handle);
			AbilitySystem->ClearAbility(AbilitySpecification.Handle);
		}
	}
}
