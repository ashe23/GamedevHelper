// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhAssetScanSettings.h"

#if WITH_EDITOR
void UGdhAssetScanSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
