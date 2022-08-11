// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerSettings.generated.h"

UCLASS(Config = EditorPerProjectUserSettings)
class UGamedevHelperRenderingManagerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperRenderingManagerSettings();

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

	UFUNCTION()
	FString GetErrorText() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	bool IsValid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="RendererSettings|General", DisplayName="FFmpeg .exe path", meta = (ToolTip = "FFmpeg executable path on system"))
	FFilePath FFmpegExePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|General", DisplayName = "Output directory", meta = (ToolTip = "Renderer output directory"))
	FDirectoryPath OutputDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Resolution", meta = (ToolTip = "Resolution presets"))
	EGamedevHelperRendererResolutionPreset ResolutionPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Custom Resolution",
		meta = (
			ToolTip = "Max supported resolution: 4096",
			EditCondition = "ResolutionPreset == EGamedevHelperRendererResolutionPreset::ResCustom",
			UIMin="1", UIMax="4096",
			ClampMin="1", ClampMax="4096"))
	FIntPoint CustomResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Video Format", meta = (ToolTip = "Output video format"))
	EGamedevHelperRendererVideoFormat VideoFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Image Format", meta = (ToolTip = "Output image format"))
	EGamedevHelperRendererImageFormat ImageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "RendererSettings|Format", DisplayName = "Framerate", meta = (ToolTip = "Output framerate"))
	FFrameRate Framerate;
protected:

	void CreateOutputSubdirectories() const;
private:
	UPROPERTY()
	FIntPoint Resolution;

	UPROPERTY()
	FString ErrorText;
};