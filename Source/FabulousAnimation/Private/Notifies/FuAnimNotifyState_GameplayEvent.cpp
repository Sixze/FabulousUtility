#include "Notifies/FuAnimNotifyState_GameplayEvent.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/FuAbilitySystemUtility.h"
#include "Utility/FuEventDataUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnimNotifyState_GameplayEvent)

namespace FuGameplayEventStateAnimationNotify
{
	static const auto* AllowPredictiveEffectsConsoleVariable{
		IConsoleManager::Get().FindConsoleVariable(TEXT("AbilitySystem.Fix.AllowPredictiveGEFlags"))
	};

	static bool IsPredictiveApplyEffectsByEventAllowed()
	{
		return AllowPredictiveEffectsConsoleVariable != nullptr &&
		       (AllowPredictiveEffectsConsoleVariable->GetInt() & 4) > 0;
	}
}

UFuAnimNotifyState_GameplayEvent::UFuAnimNotifyState_GameplayEvent()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
#endif

	bIsNativeBranchingPoint = true;
}

FString UFuAnimNotifyState_GameplayEvent::GetNotifyName_Implementation() const
{
	TStringBuilder<192> NotifyNameBuilder{InPlace, TEXTVIEW("Fu Gameplay Event: ")};

#if WITH_EDITOR
	if (!CustomDescription.IsNone())
	{
		NotifyNameBuilder << CustomDescription;
	}
	else
#endif
	{
		NotifyNameBuilder << BeginEventTag.GetTagName() << TEXTVIEW(" - ") << EndEventTag.GetTagName();
	}

	// For some reason editor cuts off some characters at the end of the string, so to avoid this we insert a bunch of spaces.
	// TODO Check the need for this hack in future engine versions.

	NotifyNameBuilder << TEXTVIEW("                                ");

	return FString{NotifyNameBuilder};
}

void UFuAnimNotifyState_GameplayEvent::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                                   const float Duration, const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::NotifyBegin(Mesh, Sequence, Duration, NotifyEventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (!UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		return;
	}

	auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
	EventData.OptionalObject = Sequence;

	if (FuGameplayEventStateAnimationNotify::IsPredictiveApplyEffectsByEventAllowed())
	{
		// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(BeginEventTag, &EventData);
	}
	else
	{
		AbilitySystem->HandleGameplayEvent(BeginEventTag, &EventData);
	}
}

void UFuAnimNotifyState_GameplayEvent::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                                 const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::NotifyEnd(Mesh, Sequence, NotifyEventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (!UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		return;
	}

	auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
	EventData.OptionalObject = Sequence;

	if (FuGameplayEventStateAnimationNotify::IsPredictiveApplyEffectsByEventAllowed())
	{
		// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(EndEventTag, &EventData);
	}
	else
	{
		AbilitySystem->HandleGameplayEvent(EndEventTag, &EventData);
	}
}
