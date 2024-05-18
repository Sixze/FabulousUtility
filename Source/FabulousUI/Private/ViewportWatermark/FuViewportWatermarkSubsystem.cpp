#include "ViewportWatermark/FuViewportWatermarkSubsystem.h"

#if !UE_SERVER

#include "Engine/GameViewportClient.h"
#include "ViewportWatermark/FuViewportWatermarkSettings.h"
#include "ViewportWatermark/SFuViewportWatermark.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuViewportWatermarkSubsystem)

bool UFuViewportWatermarkSubsystem::ShouldCreateSubsystem(UObject* OuterObject) const
{
	return !CastChecked<UGameInstance>(OuterObject)->IsDedicatedServerInstance() && Super::ShouldCreateSubsystem(OuterObject);
}

void UFuViewportWatermarkSubsystem::Initialize(FSubsystemCollectionBase& SubsystemCollection)
{
	Super::Initialize(SubsystemCollection);

	if (!GetDefault<UFuViewportWatermarkSettings>()->bEnabled)
	{
		return;
	}

	if (IsValid(GetGameInstance()->GetGameViewportClient()))
	{
		GameViewport_OnViewportCreated();
	}
	else
	{
		UGameViewportClient::OnViewportCreated().AddUObject(this, &ThisClass::GameViewport_OnViewportCreated);
	}
}

void UFuViewportWatermarkSubsystem::Deinitialize()
{
	UGameViewportClient::OnViewportCreated().RemoveAll(this);

	Super::Deinitialize();
}

void UFuViewportWatermarkSubsystem::GameViewport_OnViewportCreated() const
{
	auto* Viewport{GetGameInstance()->GetGameViewportClient()};
	if (IsValid(Viewport))
	{
		UGameViewportClient::OnViewportCreated().RemoveAll(this);

		Viewport->AddViewportWidgetContent(SNew(SFuViewportWatermark), GetDefault<UFuViewportWatermarkSettings>()->ZOrder);
	}
}

#endif
