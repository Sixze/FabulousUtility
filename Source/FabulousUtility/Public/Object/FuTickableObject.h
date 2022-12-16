#pragma once

#include "FuObject.h"
#include "Tickable.h"
#include "FuTickableObject.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuTickableObject : public UFuObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual ETickableTickType GetTickableTickType() const override;

	virtual TStatId GetStatId() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Fu Tickable Object", DisplayName = "Tick")
	void ReceiveTick(float DeltaTime);
};
