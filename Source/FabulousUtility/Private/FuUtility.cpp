#include "FuUtility.h"

FString UFuUtility::GetObjectName(const TSoftObjectPtr<UObject> SoftObjectReference)
{
	return FPackageName::ObjectPathToObjectName(SoftObjectReference.ToString());
}
