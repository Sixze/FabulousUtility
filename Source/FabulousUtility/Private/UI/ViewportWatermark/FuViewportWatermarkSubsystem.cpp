#include "UI/ViewportWatermark/FuViewportWatermarkSubsystem.h"

#if !UE_SERVER

#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "UI/ViewportWatermark/FuViewportWatermarkSettings.h"
#include "UI/ViewportWatermark/FuViewportWatermarkWidget.h"

bool UFuViewportWatermarkSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return !CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance();
}

void UFuViewportWatermarkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!GetDefault<UFuViewportWatermarkSettings>()->bEnabled)
	{
		return;
	}

	if (IsValid(GetGameInstance()->GetGameViewportClient()))
	{
		OnViewportCreated();
	}
	else
	{
		UGameViewportClient::OnViewportCreated().AddUObject(this, &ThisClass::OnViewportCreated);
	}
}

void UFuViewportWatermarkSubsystem::OnViewportCreated() const
{
	auto* Viewport{GetGameInstance()->GetGameViewportClient()};
	if (IsValid(Viewport))
	{
		UGameViewportClient::OnViewportCreated().RemoveAll(this);

		Viewport->AddViewportWidgetContent(SNew(SFuViewportWatermarkWidget), GetDefault<UFuViewportWatermarkSettings>()->WidgetZOrder);
	}
}

#endif
