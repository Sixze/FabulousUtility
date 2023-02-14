#include "AbilitySystem/AbilityTasks/FuAbilityTask_AttributeListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "AbilitySystem/Utility/FuAttributeUtility.h"

UFuAbilityTask_AttributeListener* UFuAbilityTask_AttributeListener::FuWaitForAttributeChange(
	UGameplayAbility* OwningAbility, const FGameplayAttribute Attribute)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(Attribute.IsValid()) && FU_ENSURE(!Attribute.IsSystemAttribute()))
	{
		Task->Attributes1.Add(Attribute);
	}

	return Task;
}

UFuAbilityTask_AttributeListener* UFuAbilityTask_AttributeListener::FuWaitForAttributesChange(
	UGameplayAbility* OwningAbility, const TArray<FGameplayAttribute>& Attributes)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	for (const auto& Attribute : Attributes)
	{
		if (FU_ENSURE(Attribute.IsValid()) && FU_ENSURE(!Attribute.IsSystemAttribute()))
		{
			Task->Attributes1.Add(Attribute);
		}
	}

	return Task;
}

void UFuAbilityTask_AttributeListener::Activate()
{
	Super::Activate();

	if (Attributes1.IsEmpty())
	{
		EndTask();
		return;
	}

	for (const auto& Attribute : Attributes1)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
		                      .AddUObject(this, &ThisClass::AbilitySystem_OnAttributeChanged);
	}

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	float Value;

	for (const auto& Attribute : Attributes1)
	{
		if (UFuAttributeUtility::TryGetAttributeValue(AbilitySystemComponent.Get(), Attribute, Value))
		{
			OnAttributeChanged.Broadcast(Attribute, Value, Value);
		}
	}
}

void UFuAbilityTask_AttributeListener::OnDestroy(const bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		for (const auto& Attribute : Attributes1)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UFuAbilityTask_AttributeListener::AbilitySystem_OnAttributeChanged(const FOnAttributeChangeData& ChangeData) const
{
	if (ShouldBroadcastAbilityTaskDelegates() && ChangeData.NewValue != ChangeData.OldValue)
	{
		OnAttributeChanged.Broadcast(ChangeData.Attribute, ChangeData.NewValue, ChangeData.OldValue);
	}
}
