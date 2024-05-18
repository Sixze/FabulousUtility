#pragma once

#include "FuMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/PackageName.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "FuObjectUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuObjectUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Object Utility",
		Meta = (DeterminesOutputType = "Class", ReturnDisplayName = "Default Object"))
	static UObject* GetDefaultObject(TSubclassOf<UObject> Class);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Object Utility",
		Meta = (AutoCreateRefTerm = "Object", ReturnDisplayName = "Object Name"))
	static FString GetObjectNameSoft(const TSoftObjectPtr<UObject>& Object);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Object Utility",
		Meta = (AutoCreateRefTerm = "Name", DeterminesOutputType = "Object", ReturnDisplayName = "Object"))
	static UObject* DuplicateObject(const UObject* Object, UObject* OuterObject, const FName& Name = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Object Utility",
		DisplayName = "Does Implement Interface (Expanded)", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool DoesImplementInterfaceExpanded(const UObject* Object, TSubclassOf<UInterface> InterfaceClass);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|Object Utility", DisplayName = "Does Implement Interface Soft (Expanded)",
		Meta = (AutoCreateRefTerm = "InterfaceClass", ExpandBoolAsExecs = "ReturnValue"))
	static bool DoesImplementInterfaceSoftExpanded(const UObject* Object, const TSoftClassPtr<UInterface>& InterfaceClass);
};

inline UObject* UFuObjectUtility::GetDefaultObject(const TSubclassOf<UObject> Class)
{
	return FU_ENSURE(IsValid(Class)) ? Class.GetDefaultObject() : nullptr;
}

inline FString UFuObjectUtility::GetObjectNameSoft(const TSoftObjectPtr<UObject>& Object)
{
	return FPackageName::ObjectPathToObjectName(Object.ToString());
}

inline UObject* UFuObjectUtility::DuplicateObject(const UObject* Object, UObject* OuterObject, const FName& Name)
{
	return ::DuplicateObject(Object, OuterObject, Name);
}

inline bool UFuObjectUtility::DoesImplementInterfaceExpanded(const UObject* Object, const TSubclassOf<UInterface> InterfaceClass)
{
	return FU_ENSURE(IsValid(Object)) && Object->GetClass()->ImplementsInterface(InterfaceClass);
}

inline bool UFuObjectUtility::DoesImplementInterfaceSoftExpanded(const UObject* Object, const TSoftClassPtr<UInterface>& InterfaceClass)
{
	return FU_ENSURE(IsValid(Object)) && Object->GetClass()->ImplementsInterface(InterfaceClass.Get());
}
