// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// #include "MoviePipelineGameOverrideSetting.h"
// #include "MovieRenderPipelineDataTypes.h"
#include "Gdh.h"
#include "GdhTypes.h"
#include "GdhRenderingSettings.generated.h"

class UMoviePipelineQueue;

UCLASS(Config = EditorPerProjectUserSettings, meta=(DisplayName="Rendering Settings"))
class GDH_API UGdhRenderingSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UGdhRenderingSettings();
	
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
	virtual FString GetDesc() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	bool IsValidSettings() const;
	
	FIntPoint GetResolution() const;
	FString GetResolutionFolderName() const;
	UClass* GetImageClass() const;
	FString GetImageExtension(const bool IncludeDot = false) const;
	FString GetVideoExtension(const bool IncludeDot = false) const;
	// UMoviePipelineMasterConfig* GetMasterConfig() const;
    // UMoviePipelineMasterConfig* GetMasterConfig(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="Output directory for rendered images and videos"))
	FDirectoryPath OutputDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="FFmpeg exe path", FilePathFilter="exe"))
	FFilePath FFmpegExe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered image format"))
	EGdhImageFormat ImageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered video format"))
	EGdhVideoFormat VideoFormat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered image resolutions"))
	EGdhResolutionPreset ResolutionPreset = GdhConstants::DefaultResolutionPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", DisplayName="Custom Resolution",
		meta=(
			ToolTip = "Max resolution: 4096",
			EditCondition = "ResolutionPreset == EGdhResolutionPreset::ResCustom",
			UIMin="1", UIMax="4096",
			ClampMin="1", ClampMax="4096"))
	FIntPoint CustomResolution = GdhConstants::DefaultResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings")
	FFrameRate Framerate = GdhConstants::DefaultFrameRate;
private:
	// bool ValidateOutputDirectory();
	// bool ValidateFFmpegExe();
	// bool ValidateResolution();
	// bool ValidateMovieRenderSettings();
	// bool IsValidJobSetting(UMoviePipelineSetting* Setting);

	FIntPoint CurrentResolution = GdhConstants::DefaultResolution;
};
