#pragma once

#include "Widgets/SCompoundWidget.h"

class FABULOUSUTILITY_API SFuViewportWatermarkWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFuViewportWatermarkWidget)
	{
		_Visibility = EVisibility::HitTestInvisible;
	}

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& Arguments);
};
