// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "VideoEncoderTool/GdhVideoEncoderToolSettings.h"

UGdhVideoEncoderToolSettings::UGdhVideoEncoderToolSettings() {}

#if WITH_EDITOR
void UGdhVideoEncoderToolSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig();
}
#endif
