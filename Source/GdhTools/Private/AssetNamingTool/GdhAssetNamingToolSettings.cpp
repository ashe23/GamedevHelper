// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "AssetNamingTool/GdhAssetNamingToolSettings.h"

#if WITH_EDITOR
void UGdhAssetNamingToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig();
}
#endif
