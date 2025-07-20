#include "AbilityAsync/FuAbilityAsync_AttributeListener.h"

#include "Utility/FuAbilitySystemUtility.h"
#include "Utility/FuAttributeUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuAbilityAsync_AttributeListener)

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::ListenForAttributeChangeOnActor(
	const AActor* Actor, const FGameplayAttribute InAttribute, const bool bInSkipEqualValuesOnServer)
{
	return ListenForAttributeChange(UFuAbilitySystemUtility::GetAbilitySystem(Actor),
	                                InAttribute, bInSkipEqualValuesOnServer);
}

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::ListenForAttributesChangeOnActor(
	const AActor* Actor, const TArray<FGameplayAttribute>& InAttributes, const bool bInSkipEqualValuesOnServer)
{
	return ListenForAttributesChange(UFuAbilitySystemUtility::GetAbilitySystem(Actor),
	                                 InAttributes, bInSkipEqualValuesOnServer);
}

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystem, const FGameplayAttribute InAttribute, const bool bInSkipEqualValuesOnServer)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);

	if (FU_ENSURE(InAttribute.IsValid()) && FU_ENSURE(!InAttribute.IsSystemAttribute()))
	{
		Task->Attributes.Emplace(InAttribute);
	}

	Task->bSkipEqualValuesOnServer = bInSkipEqualValuesOnServer;

	return Task;
}

UFuAbilityAsync_AttributeListener* UFuAbilityAsync_AttributeListener::ListenForAttributesChange(
	UAbilitySystemComponent* AbilitySystem, const TArray<FGameplayAttribute>& InAttributes, const bool bInSkipEqualValuesOnServer)
{
	auto* Task{NewObject<ThisClass>()};
	Task->SetAbilitySystemComponent(AbilitySystem);
	Task->Attributes.Reserve(InAttributes.Num());

	for (const auto& Attribute : InAttributes)
	{
		if (FU_ENSURE(Attribute.IsValid()) && FU_ENSURE(!Attribute.IsSystemAttribute()))
		{
			Task->Attributes.Emplace(Attribute);
		}
	}

	Task->bSkipEqualValuesOnServer = bInSkipEqualValuesOnServer;

	return Task;
}

void UFuAbilityAsync_AttributeListener::Activate()
{
	Super::Activate();

	auto* AbilitySystem{GetAbilitySystemComponent()};

	if (!IsValid(AbilitySystem) || Attributes.IsEmpty())
	{
		EndAction();
		return;
	}

	for (const auto& Attribute : Attributes)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::AbilitySystem_OnAttributeChanged);
	}

	if (!ShouldBroadcastDelegates())
	{
		return;
	}

	float Value;

	for (const auto& Attribute : Attributes)
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
		for (const auto& Attribute : Attributes)
		{
			AbilitySystem->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	Super::EndAction();
}

void UFuAbilityAsync_AttributeListener::AbilitySystem_OnAttributeChanged(const FOnAttributeChangeData& ChangeData) const
{
	if (ShouldBroadcastDelegates() && (ChangeData.NewValue != ChangeData.OldValue || !bSkipEqualValuesOnServer || // NOLINT(clang-diagnostic-float-equal)
	                                   GetAbilitySystemComponent()->GetOwnerRole() <= ROLE_AutonomousProxy))
	{
		OnAttributeChanged.Broadcast(ChangeData.Attribute, ChangeData.NewValue, ChangeData.OldValue);
	}
}
