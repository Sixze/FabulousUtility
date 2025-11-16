#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuDebugUtility.generated.h"

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = true))
enum class EFuCallstackType : uint8
{
	None = 0 UMETA(Hidden),
	NativeCallstack = 1 << 0,
	ScriptCallstack = 1 << 1,
	All = NativeCallstack | ScriptCallstack
};

ENUM_CLASS_FLAGS(EFuCallstackType)

UCLASS()
class FABULOUSUTILITY_API UFuDebugUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FString GetCurrentCallstack(EFuCallstackType CallstackType = EFuCallstackType::All);

	// Internal blueprint-only functions.

private:
	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Debug Utility",
		DisplayName = "Get Current Callstack", Meta = (ReturnDisplayName = "Callstack String"))
	static FString GetCurrentCallstackBlueprint(
		UPARAM(Meta = (Bitmask, BitmaskEnum = "/Script/FabulousUtility.EFuCallstackType")) int32 CallstackType = 3);

	static FString GetCurrentCallstackInternal(EFuCallstackType CallstackType, void* ProgramCounter);
};
