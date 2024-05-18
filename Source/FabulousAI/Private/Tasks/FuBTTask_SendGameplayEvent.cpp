#include "Tasks/FuBTTask_SendGameplayEvent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Utility/FuAbilitySystemUtility.h"
#include "Utility/FuEventDataUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTTask_SendGameplayEvent)

UFuBTTask_SendGameplayEvent::UFuBTTask_SendGameplayEvent()
{
	NodeName = TEXTVIEW("Fu Send Gameplay Event");

	TargetKey.AllowNoneAsValue(true);
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetKey), AActor::StaticClass());

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

void UFuBTTask_SendGameplayEvent::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const auto* BlackboardData{GetBlackboardAsset()};
	if (FU_ENSURE(BlackboardData))
	{
		TargetKey.ResolveSelectedKey(*BlackboardData);
	}
}

FString UFuBTTask_SendGameplayEvent::GetStaticDescription() const
{
	TStringBuilder<256> DescriptionBuilder{InPlace, TEXTVIEW("Send Gameplay Event: ")};

	if (EventTag.IsValid())
	{
		DescriptionBuilder << EventTag.GetTagName();
	}

	DescriptionBuilder << LINE_TERMINATOR TEXTVIEW("Target: ") << TargetKey.SelectedKeyName;

	return FString{DescriptionBuilder};
}

#if WITH_EDITOR
FName UFuBTTask_SendGameplayEvent::GetNodeIconName() const
{
	return FName{TEXTVIEW("BTEditor.Graph.BTNode.Task.RunEQSQuery.Icon")};
}
#endif

EBTNodeResult::Type UFuBTTask_SendGameplayEvent::ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	const auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (!FU_ENSURE(IsValid(Blackboard)))
	{
		return EBTNodeResult::Failed;
	}

	const auto* TargetObject{Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID())};

	UAbilitySystemComponent* TargetAbilitySystem;
	if (!UFuAbilitySystemUtility::TryGetAbilitySystem(TargetObject, TargetAbilitySystem))
	{
		return EBTNodeResult::Failed;
	}

	const auto* Controller{BehaviorTree.GetAIOwner()};
	const auto* Pawn{IsValid(Controller) ? Controller->GetPawn() : nullptr};

	const auto EventData{UFuEventDataUtility::MakeEventDataFromAvatarAndAbilitySystem(Pawn, TargetAbilitySystem)};

	{
		FScopedPredictionWindow PredictionWindow{TargetAbilitySystem, true};

		TargetAbilitySystem->HandleGameplayEvent(EventTag, &EventData);
	}

	return EBTNodeResult::Succeeded;
}
