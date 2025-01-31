// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoEncoderToolSettings.generated.h"

class UGdhRenderList;
class UMoviePipelineQueue;
class UMoviePipelineMasterConfig;
class UMoviePipelineExecutorBase;

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhVideoEncoderToolSettings : public UObject
{
public:

	GENERATED_BODY()

	UGdhVideoEncoderToolSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Main output directory for rendered images and encoded videos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FDirectoryPath DirOutput;

	// Directory name for rendered images
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FString DirNameImages = TEXT("images");

	// Directory name for encoded videos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FString DirNameVideo = TEXT("video");

	// If caching enabled already rendered sequences won`t be rendered again.
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	// bool bEnableCaching = true;

	// Movie render queue settings to use when rendering images. Some of the setting tho will be overriden by us, like OutputDir, and custom playback.
	// Only following settings will be supported for now (Images Formats, Anti Aliasing, Console Variables, GameOverrides, Deferred Rendering,
	// PathTracing, Camera and UI Renderer)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TSoftObjectPtr<UMoviePipelineMasterConfig> RenderSettings;

	// List of level sequences to render. Note that settings defined in queue assets are ignored.
	// So we are using queue assets as grouped sequence list.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TArray<TSoftObjectPtr<UMoviePipelineQueue>> RenderQueues;

	// Movie render queue executor class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TSoftClassPtr<UMoviePipelineExecutorBase> ExecutorClass;

	// TODO:ashe23 this will separate asset type (pipeline_name => encode_cmd)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TMap<FString, FString> EncodePipeline;
};
