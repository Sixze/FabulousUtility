#include "FuSlateUtility.h"

#include "Components/Viewport.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/UserInterfaceSettings.h"
#include "Framework/Application/SlateApplication.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuSlateUtility)

namespace FuSlateUtility
{
	template <EFuFocusableWidgetsSearchMode SearchMode>
	static int32 FindFocusableWidgets(SWidget& RootWidget, TArray<TSharedRef<SWidget>>& FoundWidgets)
	{
		auto* Children{RootWidget.GetChildren()};
		if (Children->Num() <= 0)
		{
			return 0;
		}

		auto ResultWidgetsCount{0};

		for (auto i{0}; i < Children->Num(); i++)
		{
			auto Child{Children->GetChildAt(i)};
			if (!Child->GetVisibility().IsVisible())
			{
				continue;
			}

			if constexpr (SearchMode == EFuFocusableWidgetsSearchMode::AncestorWidgets)
			{
				if (Child->SupportsKeyboardFocus())
				{
					FoundWidgets.Emplace(Child);
					ResultWidgetsCount += 1;
				}
				else
				{
					ResultWidgetsCount += FindFocusableWidgets<SearchMode>(*Child, FoundWidgets);
				}
			}
			else if constexpr (SearchMode == EFuFocusableWidgetsSearchMode::AncestorWidgetsWithSingleDescendant)
			{
				const auto AddedWidgetsCount{FindFocusableWidgets<SearchMode>(*Child, FoundWidgets)};

				if (AddedWidgetsCount > 1)
				{
					ResultWidgetsCount += AddedWidgetsCount;
					continue;
				}

				if (AddedWidgetsCount == 1)
				{
					if (Child->SupportsKeyboardFocus())
					{
						FoundWidgets.RemoveAtSwap(FoundWidgets.Num() - 1, EAllowShrinking::No);
						FoundWidgets.Emplace(Child);
						ResultWidgetsCount += 1;
					}

					ResultWidgetsCount += AddedWidgetsCount;
					continue;
				}

				if (Child->SupportsKeyboardFocus())
				{
					FoundWidgets.Emplace(Child);
					ResultWidgetsCount += 1;
				}
			}
			else if constexpr (SearchMode == EFuFocusableWidgetsSearchMode::DescendantWidgets)
			{
				const auto AddedWidgetsCount{FindFocusableWidgets<SearchMode>(*Child, FoundWidgets)};

				if (AddedWidgetsCount > 0)
				{
					ResultWidgetsCount += AddedWidgetsCount;
					continue;
				}

				if (Child->SupportsKeyboardFocus())
				{
					FoundWidgets.Emplace(Child);
					ResultWidgetsCount += 1;
				}
			}
		}

		return ResultWidgetsCount;
	}
}

void UFuSlateUtility::PlaySound(const FSlateSound& Sound)
{
#if !UE_SERVER
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().PlaySound(Sound);
	}
#endif
}

float UFuSlateUtility::GetViewportDpiScale(FVector2D ViewportSize)
{
	const auto* UserInterfaceSettings{GetDefault<UUserInterfaceSettings>()};

	if (!ViewportSize.IsNearlyZero())
	{
		return UserInterfaceSettings->GetDPIScaleBasedOnSize({
			FMath::RoundToInt32(ViewportSize.X),
			FMath::RoundToInt32(ViewportSize.Y)
		});
	}

	if (IsValid(GEngine))
	{
		const auto* World{GEngine->GetWorld()};
		const auto* Viewport{IsValid(World) ? World->GetGameViewport() : GEngine->GameViewport.Get()};

		if (IsValid(Viewport) && Viewport->Viewport != nullptr)
		{
			ViewportSize = Viewport->Viewport->GetSizeXY();

			const auto ViewportDpiScaleInverse{1.0f / Viewport->GetDPIScale()};

			return UserInterfaceSettings->GetDPIScaleBasedOnSize({
				FMath::RoundToInt32(ViewportSize.X * ViewportDpiScaleInverse),
				FMath::RoundToInt32(ViewportSize.Y * ViewportDpiScaleInverse)
			});
		}
	}

	return UserInterfaceSettings->GetDPIScaleBasedOnSize(FIntPoint::ZeroValue);
}

void UFuSlateUtility::FindFocusableWidgets(SWidget& RootWidget, const EFuFocusableWidgetsSearchMode SearchMode,
                                           TArray<TSharedRef<SWidget>>& FoundWidgets)
{
	if (SearchMode == EFuFocusableWidgetsSearchMode::AncestorWidgets)
	{
		FuSlateUtility::FindFocusableWidgets<EFuFocusableWidgetsSearchMode::AncestorWidgets>(RootWidget, FoundWidgets);
	}
	else if (SearchMode == EFuFocusableWidgetsSearchMode::AncestorWidgetsWithSingleDescendant)
	{
		FuSlateUtility::FindFocusableWidgets<EFuFocusableWidgetsSearchMode::AncestorWidgetsWithSingleDescendant>(RootWidget, FoundWidgets);
	}
	if (SearchMode == EFuFocusableWidgetsSearchMode::DescendantWidgets)
	{
		FuSlateUtility::FindFocusableWidgets<EFuFocusableWidgetsSearchMode::DescendantWidgets>(RootWidget, FoundWidgets);
	}
}
