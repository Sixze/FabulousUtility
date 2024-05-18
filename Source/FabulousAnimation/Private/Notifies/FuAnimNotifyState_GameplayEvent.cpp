#include "Notifies/FuAnimNotifyState_GameplayEvent.h"

#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Utility/FuAbilitySystemUtility.h"
#include "Utility/FuEventDataUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAnimNotifyState_GameplayEvent)

UFuAnimNotifyState_GameplayEvent::UFuAnimNotifyState_GameplayEvent()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
#endif

	bIsNativeBranchingPoint = true;
}

FString UFuAnimNotifyState_GameplayEvent::GetNotifyName_Implementation() const
{
#if WITH_EDITOR
	if (!NotifyName.IsNone())
	{
		return NotifyName.ToString();
	}
#endif

	TStringBuilder<256> NotifyNameBuilder{
		InPlace, TEXTVIEW("Fu Gameplay Event: "), BeginEventTag.GetTagName(), TEXTVIEW(" - "), EndEventTag.GetTagName()
	};

	return FString{NotifyNameBuilder};
}

void UFuAnimNotifyState_GameplayEvent::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                                   const float Duration, const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::NotifyBegin(Mesh, Sequence, Duration, NotifyEventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
		EventData.OptionalObject = Sequence;

		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(BeginEventTag, &EventData);
	}
}

void UFuAnimNotifyState_GameplayEvent::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
                                                 const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::NotifyEnd(Mesh, Sequence, NotifyEventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
		EventData.OptionalObject = Sequence;

		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(EndEventTag, &EventData);
	}
}
