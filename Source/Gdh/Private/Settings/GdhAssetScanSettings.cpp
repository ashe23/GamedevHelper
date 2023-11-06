// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhAssetScanSettings.h"

FGdhSettingsChanged& UGdhAssetScanSettings::OnSettingsChanged()
{
	return DelegateSettingsChanged;
}

#if WITH_EDITOR
void UGdhAssetScanSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (DelegateSettingsChanged.IsBound())
	{
		DelegateSettingsChanged.Broadcast();
	}

	SaveConfig();
}
#endif
