// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/GdhBetSettings.h"

UGdhBetSettings::UGdhBetSettings() {}

void UGdhBetSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
