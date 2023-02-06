#include "AbilitySystem/AbilityAsync/FuAbilityAsync_AttributeListener.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FuMacros.h"
#include "AbilitySystem/Utility/FuAttributeUtility.h"

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::FuListenForAttributeChangeOnActor(
	const AActor* Actor, const FGameplayAttribute Attribute, const bool bSkipEqualValuesOnServer)
{
	return FuListenForAttributeChange(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor),
	                                  Attribute, bSkipEqualValuesOnServer);
}

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::FuListenForAttributesChangeOnActor(
	const AActor* Actor, const TArray<FGameplayAttribute>& Attributes, const bool bSkipEqualValuesOnServer)
{
	return FuListenForAttributesChange(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor),
	                                   Attributes, bSkipEqualValuesOnServer);
}

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::FuListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute Attribute, const bool bSkipEqualValuesOnServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(Attribute.IsValid()) && FU_ENSURE(!Attribute.IsSystemAttribute()))
	{
		Task->Attributes1.Add(Attribute);
	}

	Task->bSkipEqualValuesOnServer1 = bSkipEqualValuesOnServer;

	return Task;
}

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::FuListenForAttributesChange(
	UAbilitySystemComponent* AbilitySystem, const TArray<FGameplayAttribute>& Attributes, const bool bSkipEqualValuesOnServer)
{
	auto* Task{NewObject<ThisClass>()};

	Task->SetAbilitySystemComponent(AbilitySystem);

	for (const auto& Attribute : Attributes)
	{
		if (FU_ENSURE(Attribute.IsValid()) && FU_ENSURE(!Attribute.IsSystemAttribute()))
		{
			Task->Attributes1.Add(Attribute);
		}
	}

	Task->bSkipEqualValuesOnServer1 = bSkipEqualValuesOnServer;

	return Task;
}

void UFuAbilityAsync_AttributeListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || Attributes1.IsEmpty())
	{
		EndAction();
		return;
	}

	for (const auto& Attribute : Attributes1)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::AbilitySystem_OnAttributeChanged);
	}

	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	float Value;

	for (const auto& Attribute : Attributes1)
	{
		if (UFuAttributeUtility::TryGetAttributeValue(AbilitySystem, Attribute, Value))
		{
			OnAttributeChanged.Broadcast(Attribute, Value, Value);
		}
	}
}

void UFuAbilityAsync_AttributeListener::EndAction()
{
	auto* AbilitySystem{GetAbilitySystemComponent()};
	if (IsValid(AbilitySystem))
	{
		for (const auto& Attribute : Attributes1)
		{
			AbilitySystem->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_AttributeListener::AbilitySystem_OnAttributeChanged(const FOnAttributeChangeData& ChangeData) const
{
	if (ShouldBroadcastDelegates() && (ChangeData.NewValue != ChangeData.OldValue || !bSkipEqualValuesOnServer1 ||
	                                   GetAbilitySystemComponent()->GetOwnerRole() <= ROLE_AutonomousProxy))
	{
		OnAttributeChanged.Broadcast(ChangeData.Attribute, ChangeData.NewValue, ChangeData.OldValue);
	}
}
