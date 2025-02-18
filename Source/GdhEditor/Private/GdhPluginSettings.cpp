// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhPluginSettings.h"
#include "GdhConstants.h"
#include "GdhLibPath.h"

UGdhPluginSettings::UGdhPluginSettings() {
	FFmpegExePath.FilePath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg"));
}

FName UGdhPluginSettings::GetContainerName() const {
	return FName {TEXT("Project")};
}

FName UGdhPluginSettings::GetCategoryName() const {
	return FName {TEXT("Plugins")};
}

FName UGdhPluginSettings::GetSectionName() const {
	return GdhConstants::ModuleFullName;
}

FText UGdhPluginSettings::GetSectionText() const {
	return FText::FromName(GdhConstants::ModuleFullName);
}

#if WITH_EDITOR
void UGdhPluginSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif
