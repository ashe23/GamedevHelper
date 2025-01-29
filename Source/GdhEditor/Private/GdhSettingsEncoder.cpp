// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSettingsEncoder.h"
#include "GdhEditorModule.h"

void UGdhSettingsEncoder::VisualizerUpdate() {

	CmdVisualizer.Reset();

	for (const auto& CmdPart : Cmd) {
		CmdVisualizer.Append(FString::Printf(TEXT(" %s "), *CmdPart));
	}

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *CmdVisualizer);
}
