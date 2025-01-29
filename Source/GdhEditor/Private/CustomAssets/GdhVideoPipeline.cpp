// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CustomAssets/GdhVideoPipeline.h"
#include "GdhEditorModule.h"

void UGdhVideoPipeline::EncodeCmdUpdate() {

	EncodeCmdVis.Reset();

	for (const auto& CmdPart : EncodeCmd) { EncodeCmdVis.Append(FString::Printf(TEXT(" %s "), *CmdPart)); }

	UE_LOG(LogGdhEditor, Warning, TEXT("%s"), *EncodeCmdVis);
}
