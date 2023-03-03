#include "Object/FuTickableObject.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuTickableObject)

void UFuTickableObject::Tick(const float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

ETickableTickType UFuTickableObject::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

TStatId UFuTickableObject::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFuTickableObject, STATGROUP_Tickables)
}

UWorld* UFuTickableObject::GetTickableGameObjectWorld() const
{
	return GetWorld();
}
