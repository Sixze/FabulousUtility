#pragma once

#include "Widgets/SCompoundWidget.h"

class FABULOUSUTILITY_API SFuViewportWatermark : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFuViewportWatermark)
	{
		_Visibility = EVisibility::HitTestInvisible;
	}

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& Arguments);
};
