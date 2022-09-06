#pragma once

#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "FuObjectUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuObjectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu Object Utility", Meta = (DeterminesOutputType = "Class"))
	static UObject* GetDefaultObject(TSubclassOf<UObject> Class);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Fu Object Utility",
		Meta = (DefaultToSelf = "Object", ExpandBoolAsExecs = "ReturnValue"))
	static bool SwitchImplementsInterface(const UObject* Object, TSubclassOf<UInterface> InterfaceClass);
};

inline UObject* UFuObjectUtility::GetDefaultObject(const TSubclassOf<UObject> Class)
{
	return FU_ENSURE(IsValid(Class)) ? Class.GetDefaultObject() : nullptr;
}

inline bool UFuObjectUtility::SwitchImplementsInterface(const UObject* Object, const TSubclassOf<UInterface> InterfaceClass)
{
	return FU_ENSURE(IsValid(Object)) && Object->GetClass()->ImplementsInterface(InterfaceClass);
}
