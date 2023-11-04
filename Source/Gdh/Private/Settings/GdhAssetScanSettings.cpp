// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhAssetScanSettings.h"

#if WITH_EDITOR
void UGdhAssetScanSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
