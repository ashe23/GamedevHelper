// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoEncoderToolSettings.generated.h"

class UGdhRenderList;
class UMoviePipelineQueue;
class UMoviePipelineMasterConfig;

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhVideoEncoderToolSettings : public UObject
{
public:

	GENERATED_BODY()

	UGdhVideoEncoderToolSettings();

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Config,
		Category = "General",
		meta = (ToolTip = "Main output directory for rendered images and encoded videos")
	)
	FDirectoryPath DirOutput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FString DirNameImages = TEXT("images");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FString DirNameVideo = TEXT("video");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TSoftObjectPtr<UMoviePipelineMasterConfig> RenderSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TArray<TSoftObjectPtr<UMoviePipelineQueue>> RenderQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Render")
	TArray<TSoftObjectPtr<UGdhRenderList>> RenderLists;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
