#include "GameplayCue/FuGameplayCueUtility.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "Utility/FuAbilitySystemUtility.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(FuGameplayCueUtility)

TArray<AActor*> UFuGameplayCueUtility::GetActors(const FGameplayCueParameters& Parameters)
{
	TArray<AActor*> ResultActors;

	const auto* EffectContext{Parameters.EffectContext.Get()};
	if (EffectContext == nullptr)
	{
		return ResultActors;
	}

	for (const auto& Actor : EffectContext->GetActors())
	{
		ResultActors.Add(Actor.Get());
	}

	return ResultActors;
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCueOnActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
                                                           const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		ExecuteLocalGameplayCue(AbilitySystem, GameplayCueTag, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(Actor, GameplayCueTag,
		                                                                        EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCueToActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
                                                       const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		AddLocalGameplayCue(AbilitySystem, GameplayCueTag, Parameters);
	}
	else
	{
		auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

		CueManager->HandleGameplayCue(Actor, GameplayCueTag, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCue(Actor, GameplayCueTag, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCueFromActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
                                                            const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		RemoveLocalGameplayCue(AbilitySystem, GameplayCueTag, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(Actor, GameplayCueTag,
		                                                                        EGameplayCueEvent::Removed, Parameters);
	}
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCuesOnActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
                                                            const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		ExecuteLocalGameplayCues(AbilitySystem, GameplayCueTags, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCues(Actor, GameplayCueTags,
		                                                                         EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCuesToActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
                                                        const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		AddLocalGameplayCues(AbilitySystem, GameplayCueTags, Parameters);
	}
	else
	{
		auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

		CueManager->HandleGameplayCues(Actor, GameplayCueTags, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCues(Actor, GameplayCueTags, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCuesFromActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
                                                             const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		RemoveLocalGameplayCues(AbilitySystem, GameplayCueTags, Parameters);
	}
	else
	{
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCues(Actor, GameplayCueTags,
		                                                                         EGameplayCueEvent::Removed, Parameters);
	}
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
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

		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::Executed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
                                                const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->AddLooseGameplayTag(GameplayCueTag);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::OnActive, ParametersCopy);
		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::WhileActive, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
                                                   const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->RemoveLooseGameplayTag(GameplayCueTag);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::Removed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCue(Avatar, GameplayCueTag, EGameplayCueEvent::Removed, Parameters);
	}
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
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

		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::Executed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::Executed, Parameters);
	}
}

void UFuGameplayCueUtility::AddLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
                                                 const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->AddLooseGameplayTags(GameplayCueTags);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::OnActive, ParametersCopy);
		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::WhileActive, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::OnActive, Parameters);
		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
                                                    const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	AbilitySystem->RemoveLooseGameplayTags(GameplayCueTags);

	auto* CueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	if (!Parameters.EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());

		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::Removed, ParametersCopy);
	}
	else
	{
		CueManager->HandleGameplayCues(Avatar, GameplayCueTags, EGameplayCueEvent::Removed, Parameters);
	}
}
