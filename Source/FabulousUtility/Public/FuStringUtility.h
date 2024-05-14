#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuStringUtility.generated.h"

UCLASS()
class FABULOUSUTILITY_API UFuStringUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static constexpr FStringView BoolToString(bool bValue);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|String Utility",
		Meta = (AutoCreateRefTerm = "String", ReturnDisplayName = "Display String"))
	static FString ToDisplayString(const FString& String, bool bStringIsBool);

	UFUNCTION(BlueprintPure, Category = "Fabulous Utility|String Utility", DisplayName = "To Formatted String (Float)",
		Meta = (ReturnDisplayName = "Formatted String", AdvancedDisplay = 1))
	static FString FloatToFormattedString(double Value, TEnumAsByte<ERoundingMode> RoundingMode, bool bAlwaysSign = false,
	                                      bool bUseGrouping = true, int32 MinIntegralDigits = 1, int32 MaxIntegralDigits = 324,
	                                      int32 MinFractionalDigits = 0, int32 MaxFractionalDigits = 3);
};

constexpr FStringView UFuStringUtility::BoolToString(const bool bValue)
{
	return bValue ? TEXTVIEW("True") : TEXTVIEW("False");
}
