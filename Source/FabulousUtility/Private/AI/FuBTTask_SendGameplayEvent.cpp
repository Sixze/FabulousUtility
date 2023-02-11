#include "AI/FuBTTask_SendGameplayEvent.h"

#include "AIController.h"
#include "AbilitySystem/Utility/FuAbilitySystemUtility.h"
#include "AbilitySystem/Utility/FuEventDataUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuBTTask_SendGameplayEvent)

UFuBTTask_SendGameplayEvent::UFuBTTask_SendGameplayEvent()
{
	NodeName = TEXT("Fu Send Gameplay Event");

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
	return FString::Printf(TEXT("Send Gameplay Event: %s") LINE_TERMINATOR TEXT("Target: %s"),
	                       *EventTag.ToString(), *TargetKey.SelectedKeyName.ToString());
}

#if WITH_EDITOR
FName UFuBTTask_SendGameplayEvent::GetNodeIconName() const
{
	return TEXT("BTEditor.Graph.BTNode.Task.RunEQSQuery.Icon");
}
#endif

EBTNodeResult::Type UFuBTTask_SendGameplayEvent::ExecuteTask(UBehaviorTreeComponent& BehaviorTree, uint8* NodeMemory)
{
	const auto* Blackboard{BehaviorTree.GetBlackboardComponent()};
	if (!FU_ENSURE(IsValid(Blackboard)))
	{
		return EBTNodeResult::Failed;
	}

	const auto* Target{Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID())};

	UAbilitySystemComponent* TargetAbilitySystem;
	if (!UFuAbilitySystemUtility::TryGetAbilitySystem(Target, TargetAbilitySystem))
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
