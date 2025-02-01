// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ActorNamingTool/GdhAntSettings.h"

UGdhAntSettings::UGdhAntSettings() {}

#if WITH_EDITOR
void UGdhAntSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
