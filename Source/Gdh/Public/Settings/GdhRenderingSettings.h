// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gdh.h"
#include "GdhTypes.h"
#include "GdhRenderingSettings.generated.h"

class UMoviePipelineQueue;

DECLARE_DELEGATE(FGdhRenderingSettingsOnChangeDelegate);

UCLASS(Config = EditorPerProjectUserSettings, meta=(DisplayName="Rendering Settings"))
class GDH_API UGdhRenderingSettings final : public UDeveloperSettings
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

	FIntPoint GetResolution() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="Output directory for rendered images and videos"))
	FDirectoryPath OutputDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="FFmpeg exe path. Can be just ffmpeg.exe, if you have already installed it on system", FilePathFilter="exe"))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="EncodingSettings",
		meta=(ToolTip="FFmpeg command input flags, that will be used when encoding video. You can see complete command in FFmpegEncodeCmdPreview property"))
	TArray<FString> FFmpegFlags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EncodingSettings",
		meta=(ToolTip=
			"This is encode command preview, that will be used when encoding videos. Image input, Video output paths, framerate and resolution are generated automatically, the rest you can change via FFmpegFlags property. {ffmpeg_exe_path} is one you specified in FFmpegExe property"
		))
	FString FFmpegEncodeCmdPreview;

	FGdhRenderingSettingsOnChangeDelegate& OnChange();

private:
	FIntPoint CurrentResolution = GdhConstants::DefaultResolution;
	FGdhRenderingSettingsOnChangeDelegate OnChangeDelegate;
};
