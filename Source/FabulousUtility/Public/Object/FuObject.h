#pragma once

#include "FuMacros.h"
#include "GameFramework/Actor.h"
#include "FuObject.generated.h"

UCLASS(Blueprintable, BlueprintType, AutoExpandCategories = ("Settings|Fu Object", "State|Fu Object"))
class FABULOUSUTILITY_API UFuObject : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Fu Object", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AActor> Owner;

public:
	virtual void PostInitProperties() override;

	virtual UWorld* GetWorld() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#if UE_WITH_IRIS
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
	                                          UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif

	virtual bool IsSupportedForNetworking() const override;

	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParameters, FFrame* Stack) override;

	// 	void CallPreReplication();
	//
	// protected:
	// 	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker);

public:
	AActor* GetOwner() const;

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Object", Meta = (ReturnDisplayName = "Local Role"))
	ENetRole GetOwnerLocalRole() const;

protected:
	virtual void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent, Category = "Fu Object", DisplayName = "Begin Play")
	void ReceiveBeginPlay();

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Object")
	void Destroy();
};

inline AActor* UFuObject::GetOwner() const
{
	return Owner;
}

inline ENetRole UFuObject::GetOwnerLocalRole() const
{
	return FU_ENSURE(IsValid(GetOwner())) ? GetOwner()->GetLocalRole() : ROLE_None;
}

inline void UFuObject::Destroy()
{
	MarkAsGarbage();
}
