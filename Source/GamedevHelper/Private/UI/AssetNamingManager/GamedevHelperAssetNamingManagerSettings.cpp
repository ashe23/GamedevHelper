// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"

UGamedevHelperAssetNamingManagerSettings::UGamedevHelperAssetNamingManagerSettings()
{
	SetDefaultSettings();
}

#if WITH_EDITOR
void UGamedevHelperAssetNamingManagerSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (OnSettingsChangeDelegate.IsBound())
	{
		OnSettingsChangeDelegate.Execute();
	}
}
#endif

void UGamedevHelperAssetNamingManagerSettings::SetDefaultSettings()
{
	ScanPath.Path = TEXT("/Game");
	bScanRecursive = true;
	bShowMissingTypes = false;
}
