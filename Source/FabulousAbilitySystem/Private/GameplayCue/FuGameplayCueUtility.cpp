#include "GameplayCue/FuGameplayCueUtility.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "Utility/FuAbilitySystemUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayCueUtility)

TArray<AActor*> UFuGameplayCueUtility::GetActors(const FGameplayCueParameters& Parameters)
{
	TArray<AActor*> Actors;

	const auto* EffectContext{Parameters.EffectContext.Get()};
	if (EffectContext == nullptr)
	{
		return Actors;
	}

	Actors.Reserve(EffectContext->GetActors().Num());

	for (const auto& Actor : EffectContext->GetActors())
	{
		Actors.Emplace(Actor.Get());
	}

	return Actors;
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCueOnActor(AActor* Actor, const FGameplayTag& CueTag,
                                                           const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		ExecuteLocalGameplayCue(AbilitySystem, CueTag, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(Actor, CueTag,
		                                                                        EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCueToActor(AActor* Actor, const FGameplayTag& CueTag,
                                                       const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		AddLocalGameplayCue(AbilitySystem, CueTag, Parameters);
	}
	else
	{
		auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

		CueManager->HandleGameplayCue(Actor, CueTag, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCue(Actor, CueTag, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCueFromActor(AActor* Actor, const FGameplayTag& CueTag,
                                                            const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		RemoveLocalGameplayCue(AbilitySystem, CueTag, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(Actor, CueTag,
		                                                                        EGameplayCueEvent::Removed, Parameters);
	}
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCuesOnActor(AActor* Actor, const FGameplayTagContainer& CueTags,
                                                            const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		ExecuteLocalGameplayCues(AbilitySystem, CueTags, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCues(Actor, CueTags,
		                                                                         EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCuesToActor(AActor* Actor, const FGameplayTagContainer& CueTags,
                                                        const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		AddLocalGameplayCues(AbilitySystem, CueTags, Parameters);
	}
	else
	{
		auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

		CueManager->HandleGameplayCues(Actor, CueTags, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCues(Actor, CueTags, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCuesFromActor(AActor* Actor, const FGameplayTagContainer& CueTags,
                                                             const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		RemoveLocalGameplayCues(AbilitySystem, CueTags, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCues(Actor, CueTags,
		                                                                         EGameplayCueEvent::Removed, Parameters);
	}
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& CueTag,
                                                    const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::Executed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& CueTag,
                                                const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->AddLooseGameplayTag(CueTag);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::OnActive, ParametersCopy);
		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::WhileActive, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& CueTag,
                                                   const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->RemoveLooseGameplayTag(CueTag);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::Removed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCue(Avatar, CueTag, EGameplayCueEvent::Removed, Parameters);
	}
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& CueTags,
                                                     const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::Executed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& CueTags,
                                                 const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->AddLooseGameplayTags(CueTags);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::OnActive, ParametersCopy);
		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::WhileActive, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& CueTags,
                                                    const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->RemoveLooseGameplayTags(CueTags);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::Removed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCues(Avatar, CueTags, EGameplayCueEvent::Removed, Parameters);
	}
}
