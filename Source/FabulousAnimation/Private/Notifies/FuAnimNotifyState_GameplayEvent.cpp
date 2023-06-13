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

	TStringBuilder<256> NotifyNameBuilder;

	NotifyNameBuilder << TEXTVIEW("Fu Gameplay Event: ") << BeginEventTag.GetTagName() << TEXTVIEW(" - ") << EndEventTag.GetTagName();

	return FString{NotifyNameBuilder};
}

void UFuAnimNotifyState_GameplayEvent::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, const float Duration,
                                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(Mesh, Animation, Duration, EventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
		EventData.OptionalObject = Animation;

		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(BeginEventTag, &EventData);
	}
}

void UFuAnimNotifyState_GameplayEvent::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
                                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(Mesh, Animation, EventReference);

	const auto* Owner{Mesh->GetOwner()};

	UAbilitySystemComponent* AbilitySystem;
	if (UFuAbilitySystemUtility::TryGetAbilitySystem(Owner, AbilitySystem))
	{
		auto EventData{UFuEventDataUtility::MakeEventDataFromAbilitySystems(AbilitySystem, AbilitySystem)};
		EventData.OptionalObject = Animation;

		FScopedPredictionWindow PredictionWindow{AbilitySystem, true};

		AbilitySystem->HandleGameplayEvent(EndEventTag, &EventData);
	}
}
