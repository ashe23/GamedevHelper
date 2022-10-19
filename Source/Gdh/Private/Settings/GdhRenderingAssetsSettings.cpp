// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhRenderingAssetsSettings.h"

#if WITH_EDITOR
void UGdhRenderingAssetsSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();

	if (OnChangeDelegate.IsBound())
	{
		OnChangeDelegate.Execute();
	}
}
#endif

FGdhRenderingAssetsSettingsOnChangeDelegate& UGdhRenderingAssetsSettings::OnChange()
{
	return OnChangeDelegate;
}
