#pragma once

#include "Widgets/SCompoundWidget.h"

class FABULOUSUI_API SFuViewportWatermark : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFuViewportWatermark)
	{
		_Visibility = EVisibility::HitTestInvisible;
	}

	SLATE_END_ARGS()

public:
	SFuViewportWatermark();

	void Construct(const FArguments& Arguments);
};
