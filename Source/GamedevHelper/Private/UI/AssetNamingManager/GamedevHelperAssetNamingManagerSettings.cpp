// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"

UGamedevHelperAssetNamingManagerSettings::UGamedevHelperAssetNamingManagerSettings()
{
	ScanPath.Path = TEXT("/Game");
	bScanRecursive = true;
	bShowMissingTypes = false;
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
