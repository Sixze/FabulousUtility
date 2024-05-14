#include "AbilityTasks/FuAbilityTask_AttributeListener.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"
#include "Utility/FuAttributeUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityTask_AttributeListener)

UFuAbilityTask_AttributeListener* UFuAbilityTask_AttributeListener::WaitForAttributeChange(UGameplayAbility* OwningAbility,
                                                                                           const FGameplayAttribute InAttribute)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};

	if (FU_ENSURE(InAttribute.IsValid()) && FU_ENSURE(!InAttribute.IsSystemAttribute()))
	{
		Task->Attributes.Emplace(InAttribute);
	}

	return Task;
}

UFuAbilityTask_AttributeListener* UFuAbilityTask_AttributeListener::WaitForAttributesChange(UGameplayAbility* OwningAbility,
                                                                                            const TArray<FGameplayAttribute>& InAttributes)
{
	auto* Task{NewAbilityTask<ThisClass>(OwningAbility)};
	Task->Attributes.Reserve(InAttributes.Num());

	for (const auto& Attribute : InAttributes)
	{
		if (FU_ENSURE(Attribute.IsValid()) && FU_ENSURE(!Attribute.IsSystemAttribute()))
		{
			Task->Attributes.Emplace(Attribute);
		}
	}

	return Task;
}

void UFuAbilityTask_AttributeListener::Activate()
{
	Super::Activate();

	if (Attributes.IsEmpty())
	{
		EndTask();
		return;
	}

	for (const auto& Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
		                      .AddUObject(this, &ThisClass::AbilitySystem_OnAttributeChanged);
	}

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	float Value;

	for (const auto& Attribute : Attributes)
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
		for (const auto& Attribute : Attributes)
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
