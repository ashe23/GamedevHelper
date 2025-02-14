// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhBetSettings.generated.h"

/**
 * Batch Encode Tool Main Settings class
 */
UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhBetSettings : public UObject
{
public:

	GENERATED_BODY()

	UGdhBetSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// FFmpeg executable path. Plugin will try to detect it automatically from system env PATHS. But
	// you can modify it manually.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FFilePath FFmpegExePath;

	// Main output directory for rendered images and encoded videos.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General", DisplayName = "OutputDir")
	FDirectoryPath DirOutput;
};
