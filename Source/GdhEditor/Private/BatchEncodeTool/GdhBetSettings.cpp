// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/GdhBetSettings.h"
#include "GdhLibPath.h"

UGdhBetSettings::UGdhBetSettings() {
	const FString DirSaved = FPaths::ProjectSavedDir();
	const FString DirGdh = TEXT("GdhRenders");

	DirOutput.Path = FPaths::ConvertRelativePathToFull(DirSaved / DirGdh);
	DirOutputFmt = TEXT("{dir_output}/{list}/videos/{preset}/{track}/{sequence}");
	FFmpegExePath.FilePath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg"));
}

void UGdhBetSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
