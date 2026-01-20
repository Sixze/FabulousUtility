#include "Notifies/FuAnimNotify_GameplayEvent.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/FuAbilitySystemUtility.h"
#include "Utility/FuEventDataUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnimNotify_GameplayEvent)

namespace FuGameplayEventAnimationNotify
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

UFuAnimNotify_GameplayEvent::UFuAnimNotify_GameplayEvent()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
#endif

	bIsNativeBranchingPoint = true;
}

FString UFuAnimNotify_GameplayEvent::GetNotifyName_Implementation() const
{
	TStringBuilder<128> NotifyNameBuilder{InPlace, TEXTVIEW("Fu Gameplay Event: ")};

#if WITH_EDITOR
	if (!CustomDescription.IsNone())
	{
		NotifyNameBuilder << CustomDescription;
	}
	else
#endif
	{
		NotifyNameBuilder << EventTag.GetTagName();
	}

	// For some reason editor cuts off some characters at the end of the string, so to avoid this we insert a bunch of spaces.
	// TODO Check the need for this hack in future engine versions.

	NotifyNameBuilder << TEXTVIEW("                ");

	return FString{NotifyNameBuilder};
}

void UFuAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                         const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::Notify(Mesh, Sequence, NotifyEventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (!UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		return;
	}

	auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
	EventData.OptionalObject = Sequence;

	if (FuGameplayEventAnimationNotify::IsPredictiveApplyEffectsByEventAllowed())
	{
		// ReSharper disable once CppLocalVariableWithNonTrivialDtorIsNeverUsed
		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(EventTag, &EventData);
	}
	else
	{
		AbilitySystem->HandleGameplayEvent(EventTag, &EventData);
	}
}
