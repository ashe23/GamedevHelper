// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhAssetNamingToolSettings.h"

#if WITH_EDITOR
void UGdhAssetNamingToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
