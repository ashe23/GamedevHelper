// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRendererSettings.generated.h"

UCLASS(Config = EditorPerProjectUserSettings)
class UGamedevHelperRendererSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperRendererSettings();

	UFUNCTION()
	FIntPoint GetResolution() const;

	UFUNCTION()
	FString GetResolutionAsString(const FString& Separator = TEXT(":")) const;

	UFUNCTION()
	FString GetVideoFormatAsString(const bool IncludeDot = false) const;

	UFUNCTION()
	FString GetImageFormatAsString(const bool IncludeDot = false) const;

	UFUNCTION()
	UClass* GetMoviePipelineOutputSettingImageClass() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	bool IsValid() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="RendererSettings|General", DisplayName="FFmpeg .exe path", meta = (ToolTip = "FFmpeg executable path on system"))
	FFilePath FFmpegExePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|General", DisplayName = "Output directory", meta = (ToolTip = "Renderer output directory"))
	FDirectoryPath OutputDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|General", DisplayName = "Use Movie Render Settings",
		meta = (ToolTip = "If enabled will be used movie render pipeline settings, specified in MoviePipelineQueue assets"))
	bool bUseMovieRenderPresetSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Resolution",
		meta = (ToolTip = "Resolution presets", EditCondition = "bUseMovieRenderPresetSettings == false"))
	EGamedevHelperRendererResolutionPreset ResolutionPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Custom Resolution",
		meta = (
			ToolTip = "Max supported resolution: 4096",
			EditCondition = "bCanUseCustomResolution == true",
			UIMin="1", UIMax="4096",
			ClampMin="1", ClampMax="4096"))
	FIntPoint CustomResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Video Format",
		meta = (ToolTip = "Output video format", EditCondition = "bUseMovieRenderPresetSettings == false"))
	EGamedevHelperRendererVideoFormat VideoFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Image Format",
		meta = (ToolTip = "Output image format", EditCondition = "bUseMovieRenderPresetSettings == false"))
	EGamedevHelperRendererImageFormat ImageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Framerate",
		meta = (ToolTip = "Output framerate", EditCondition = "bUseMovieRenderPresetSettings == false"))
	FFrameRate Framerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Extras", DisplayName = "Playback Speed",
		meta = (ToolTip = "Final Video Playback Speed", EditCondition = "bUseMovieRenderPresetSettings == false"))
	float PlaybackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Extras", DisplayName = "Render UI",
		meta = (ToolTip = "If enabled will render UI also", EditCondition = "bUseMovieRenderPresetSettings == false"))
	bool bRenderUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Extras", DisplayName = "Additional FFmpeg flags",
		meta = (ToolTip = "Additional ffmpeg command flags", EditCondition = "bUseMovieRenderPresetSettings == false"))
	FString AdditionalCmdFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RendererSettings|Queue", DisplayName="QueueAssets", meta=(ToolTip = "MovieRenderPipelineQueue assets", AllowedClasses="MoviePipelineQueue"))
	TArray<FSoftObjectPath> MovieRenderQueueAssets;

private:
	UPROPERTY()
	FIntPoint Resolution;

	UPROPERTY()
	bool bCanUseCustomResolution;
};
