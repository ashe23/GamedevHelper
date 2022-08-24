// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhRenderingQueueSettings.h"
// Engine Headers
void UGdhRenderingQueueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
