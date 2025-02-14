// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/GdhBetSettings.h"
#include "GdhLibPath.h"

UGdhBetSettings::UGdhBetSettings() {
	FFmpegExePath.FilePath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg"));

	const FString DirSaved = FPaths::ProjectSavedDir();
	const FString DirGdh = TEXT("GdhBatchEncoder");
	DirOutput.Path = FPaths::ConvertRelativePathToFull(DirSaved / DirGdh);
}

void UGdhBetSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
