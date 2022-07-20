#include "AbilitySystem/GameplayCue/FuGameplayCueUtility.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/Utility/FuAbilitySystemUtility.h"

TArray<AActor*> UFuGameplayCueUtility::GetActorsFromCueParameters(const FGameplayCueParameters& Parameters)
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

void UFuGameplayCueUtility::ExecuteLocalGameplayCueActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
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

void UFuGameplayCueUtility::AddLocalGameplayCueActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
                                                     const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		AddLocalGameplayCue(AbilitySystem, GameplayCueTag, Parameters);
	}
	else
	{
		auto* GameplayCueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

		GameplayCueManager->HandleGameplayCue(Actor, GameplayCueTag, EGameplayCueEvent::OnActive, Parameters);
		GameplayCueManager->HandleGameplayCue(Actor, GameplayCueTag, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCueActor(AActor* Actor, const FGameplayTag& GameplayCueTag,
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

void UFuGameplayCueUtility::ExecuteLocalGameplayCuesActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
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

void UFuGameplayCueUtility::AddLocalGameplayCuesActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
                                                      const FGameplayCueParameters& Parameters)
{
	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Actor, AbilitySystem))
	{
		AddLocalGameplayCues(AbilitySystem, GameplayCueTags, Parameters);
	}
	else
	{
		auto* GameplayCueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

		GameplayCueManager->HandleGameplayCues(Actor, GameplayCueTags, EGameplayCueEvent::OnActive, Parameters);
		GameplayCueManager->HandleGameplayCues(Actor, GameplayCueTags, EGameplayCueEvent::WhileActive, Parameters);
	}
}

void UFuGameplayCueUtility::RemoveLocalGameplayCuesActor(AActor* Actor, const FGameplayTagContainer& GameplayCueTags,
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

	const auto* ResultParameters{&Parameters};

	if (!ResultParameters->EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		ResultParameters = &ParametersCopy;

		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());
	}

	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(Avatar, GameplayCueTag,
	                                                                        EGameplayCueEvent::Executed, *ResultParameters);
}

void UFuGameplayCueUtility::AddLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
                                                const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	const auto* ResultParameters{&Parameters};

	if (!ResultParameters->EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		ResultParameters = &ParametersCopy;

		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());
	}

	AbilitySystem->AddLooseGameplayTag(GameplayCueTag);

	auto* GameplayCueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	GameplayCueManager->HandleGameplayCue(Avatar, GameplayCueTag,
	                                      EGameplayCueEvent::OnActive, *ResultParameters);

	GameplayCueManager->HandleGameplayCue(Avatar, GameplayCueTag,
	                                      EGameplayCueEvent::WhileActive, *ResultParameters);
}

void UFuGameplayCueUtility::RemoveLocalGameplayCue(UAbilitySystemComponent* AbilitySystem, const FGameplayTag& GameplayCueTag,
                                                   const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	const auto* ResultParameters{&Parameters};

	if (!ResultParameters->EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		ResultParameters = &ParametersCopy;

		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());
	}

	AbilitySystem->RemoveLooseGameplayTag(GameplayCueTag);

	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(Avatar, GameplayCueTag,
	                                                                        EGameplayCueEvent::Removed, *ResultParameters);
}

void UFuGameplayCueUtility::ExecuteLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
                                                     const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	const auto* ResultParameters{&Parameters};

	if (!ResultParameters->EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		ResultParameters = &ParametersCopy;

		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());
	}

	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCues(Avatar, GameplayCueTags,
	                                                                         EGameplayCueEvent::Executed, *ResultParameters);
}

void UFuGameplayCueUtility::AddLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
                                                 const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	const auto* ResultParameters{&Parameters};

	if (!ResultParameters->EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		ResultParameters = &ParametersCopy;

		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());
	}

	AbilitySystem->AddLooseGameplayTags(GameplayCueTags);

	auto* GameplayCueManager{UAbilitySystemGlobals::Get().GetGameplayCueManager()};

	GameplayCueManager->HandleGameplayCues(Avatar, GameplayCueTags,
	                                       EGameplayCueEvent::OnActive, *ResultParameters);

	GameplayCueManager->HandleGameplayCues(Avatar, GameplayCueTags,
	                                       EGameplayCueEvent::WhileActive, *ResultParameters);
}

void UFuGameplayCueUtility::RemoveLocalGameplayCues(UAbilitySystemComponent* AbilitySystem, const FGameplayTagContainer& GameplayCueTags,
                                                    const FGameplayCueParameters& Parameters)
{
	auto* Avatar{IsValid(AbilitySystem) ? AbilitySystem->GetAvatarActor_Direct() : nullptr};

	if (!FU_ENSURE(IsValid(Avatar)) || AbilitySystem->bSuppressGameplayCues)
	{
		return;
	}

	const auto* ResultParameters{&Parameters};

	if (!ResultParameters->EffectContext.IsValid())
	{
		auto ParametersCopy{Parameters};
		ResultParameters = &ParametersCopy;

		UAbilitySystemGlobals::Get().InitGameplayCueParameters(ParametersCopy, AbilitySystem->MakeEffectContext());
	}

	AbilitySystem->RemoveLooseGameplayTags(GameplayCueTags);

	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCues(Avatar, GameplayCueTags,
	                                                                         EGameplayCueEvent::Removed, *ResultParameters);
}
