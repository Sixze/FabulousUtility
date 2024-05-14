#include "FuStringUtility.h"

#include "Internationalization/Culture.h"
#include "Internationalization/FastDecimalFormat.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuStringUtility)

FString UFuStringUtility::ToDisplayString(const FString& String, const bool bStringIsBool)
{
	return FName::NameToDisplayString(String, bStringIsBool);
}

FString UFuStringUtility::FloatToFormattedString(const double Value, const TEnumAsByte<ERoundingMode> RoundingMode, const bool bAlwaysSign,
                                                 const bool bUseGrouping, const int32 MinIntegralDigits, const int32 MaxIntegralDigits,
                                                 const int32 MinFractionalDigits, const int32 MaxFractionalDigits)
{
	FNumberFormattingOptions Options;
	Options.AlwaysSign = bAlwaysSign;
	Options.UseGrouping = bUseGrouping;
	Options.RoundingMode = RoundingMode;
	Options.MinimumIntegralDigits = MinIntegralDigits;
	Options.MaximumIntegralDigits = MaxIntegralDigits;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;

	// FastDecimalFormat::GetCultureAgnosticFormattingRules();

	return FastDecimalFormat::NumberToString(Value, FInternationalization::Get().GetCurrentLocale()->GetDecimalNumberFormattingRules(),
	                                         Options);
}
