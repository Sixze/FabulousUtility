#include "Object/FuTickableObject.h"

void UFuTickableObject::Tick(const float DeltaTime)
{
	ReceiveTick(DeltaTime);
}

TStatId UFuTickableObject::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFuNetworkedObject, STATGROUP_Tickables)
}

UWorld* UFuTickableObject::GetTickableGameObjectWorld() const
{
	return GetWorld();
}
