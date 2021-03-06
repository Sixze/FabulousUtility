#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuStringUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuStringUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static constexpr const TCHAR* BoolToString(bool bValue);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|Fu String Utility",
		DisplayName = "To Formatted String (Float)", Meta = (AdvancedDisplay = 1))
	static FString FloatToFormattedString(double Value, TEnumAsByte<ERoundingMode> RoundingMode, bool bAlwaysSign = false,
	                                      bool bUseGrouping = true, int32 MinIntegralDigits = 1, int32 MaxIntegralDigits = 324,
	                                      int32 MinFractionalDigits = 0, int32 MaxFractionalDigits = 3);
};

constexpr const TCHAR* UFuStringUtility::BoolToString(const bool bValue)
{
	return bValue ? TEXT("True") : TEXT("False");
}
