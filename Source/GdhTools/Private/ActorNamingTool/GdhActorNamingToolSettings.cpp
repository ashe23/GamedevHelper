// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ActorNamingTool/GdhActorNamingToolSettings.h"

UGdhActorNamingToolSettings::UGdhActorNamingToolSettings() {}

#if WITH_EDITOR
void UGdhActorNamingToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
