// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhBetSettings.generated.h"

class UGdhBetEncodePreset;
class UMoviePipelineMasterConfig;
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

	// Main output directory for rendered images and encoded videos.
	// By Default will be {ProjectDir}/Saved/GdhRenders.
	UPROPERTY(
		EditAnywhere, BlueprintReadWrite, Config, Category = "General", DisplayName = "OutputDir"
	)
	FDirectoryPath DirOutput;

	// Directory structure format. Order in which should images and videos organized in final state.
	// Available Tokens are:
	// {list} - render list asset name
	// {preset} - encoding preset name
	// {sequence} - level sequence name
	// {track} - audio track name, if any
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Config,
		Category = "General",
		DisplayName = "OutputDirFormat"
	)
	FString DirOutputFmt;

	// FFmpeg executable path. Plugin will try to detect it automatically from system env PATHS. But
	// you can modify it manually.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FFilePath FFmpegExePath;

	// Which level to use when rendering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TSoftObjectPtr<UWorld> World;

	// Main Movie Render Queue settings that will be used when rendering level sequences.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TSoftObjectPtr<UMoviePipelineMasterConfig> RenderSettings;

	// Pipeline here is just means, that we will apply all encoding command that given here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Encode")
	TSet<TSoftObjectPtr<UGdhBetEncodePreset>> Pipelines;

	// Mapping of audio track name => to audio file path.
	// This mapping will be used in order to mix audio with videos.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Audio")
	TMap<FName, FFilePath> AudioTracks;
};
