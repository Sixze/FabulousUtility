#include "Object/FuObject.h"

#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/Engine.h"
#include "Engine/NetDriver.h"

// ReSharper disable once CppUnusedIncludeDirective
#include UE_INLINE_GENERATED_CPP_BY_NAME(FuObject)

void UFuObject::PostInitProperties()
{
	Super::PostInitProperties();

	Owner = GetTypedOuter<AActor>();

	if (IsValid(GetWorld()))
	{
		BeginPlay();
	}
}

UWorld* UFuObject::GetWorld() const
{
	return !HasAllFlags(RF_ClassDefaultObject) ? GetOuter()->GetWorld() : nullptr;
}

void UFuObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	const auto* BlueprintClass{Cast<UBlueprintGeneratedClass>(GetClass())};
	if (IsValid(BlueprintClass))
	{
		BlueprintClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

bool UFuObject::IsSupportedForNetworking() const
{
	return true;
}

int32 UFuObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return HasAnyFlags(RF_ClassDefaultObject) || Function->FunctionFlags & FUNC_Static
		       ? GEngine->GetGlobalFunctionCallspace(Function, this, Stack)
		       : GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UFuObject::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParameters, FFrame* Stack)
{
	auto* Actor{GetOwner()};
	if (!FU_ENSURE(IsValid(Actor)))
	{
		return false;
	}

	auto bProcessed{false};

	const auto* Context{GEngine->GetWorldContextFromWorld(GetWorld())};
	if (Context != nullptr)
	{
		for (const auto& Driver : Context->ActiveNetDrivers)
		{
			if (IsValid(Driver.NetDriver) && Driver.NetDriver->ShouldReplicateFunction(Actor, Function))
			{
				Driver.NetDriver->ProcessRemoteFunction(Actor, Function, Parameters, OutParameters, Stack, this);
				bProcessed = true;
			}
		}
	}

	return bProcessed;
}

// void UFuObject::CallPreReplication()
// {
// 	auto* Actor{GetOwner()};
// 	if (!IsValid(Actor))
// 	{
// 		return;
// 	}
//
// 	const auto* Context{GEngine->GetWorldContextFromWorld(GetWorld())};
// 	if (Context != nullptr)
// 	{
// 		for (const auto& Driver : Context->ActiveNetDrivers)
// 		{
// 			if (IsValid(Driver.NetDriver) && Driver.NetDriver->ShouldReplicateActor(Actor))
// 			{
// 				PreReplication(*Driver.NetDriver->FindOrCreateRepChangedPropertyTracker(this).Get());
// 			}
// 		}
// 	}
// }
//
// void UFuObject::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
// {
// 	const auto* BlueprintClass{Cast<UBlueprintGeneratedClass>(GetClass())};
// 	if (IsValid(BlueprintClass))
// 	{
// 		BlueprintClass->InstancePreReplication(this, ChangedPropertyTracker);
// 	}
// }

void UFuObject::BeginPlay()
{
	ReceiveBeginPlay();
}
