// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhRenderingManagerTypes.h"
#include "GdhCoreDeveloperSettings.h"
#include "GdhRenderingManager.h"
#include "GdhRenderingSettings.generated.h"

UCLASS(BlueprintType, Config = EditorPerProjectUserSettings, meta=(DisplayName="Rendering Settings"))
class GDHRENDERINGMANAGER_API UGdhRenderingSettings : public UGdhDeveloperSettings
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
	virtual void Validate() override;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FIntPoint GetResolution() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetResolutionFolderName() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	UClass* GetImageClass() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetImageExtension(const bool IncludeDot = false) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="Output directory for rendered images and videos"))
	FDirectoryPath OutputDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="FFmpeg exe path"))
	FFilePath FFmpegExe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered image format"))
	EGdhImageFormat ImageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered image resolutions"))
	EGdhResolutionPreset ResolutionPreset = GdhRenderingManagerConstants::DefaultResolutionPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", DisplayName="Custom Resolution",
		meta = (
			ToolTip = "Max resolution: 4096",
			EditCondition = "ResolutionPreset == EGdhResolutionPreset::ResCustom",
			UIMin="1", UIMax="4096",
			ClampMin="1", ClampMax="4096"))
	FIntPoint CustomResolution = GdhRenderingManagerConstants::DefaultResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered video format"))
	EGdhVideoFormat VideoFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings")
	FFrameRate Framerate = GdhRenderingManagerConstants::DefaultFrameRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Additional ffmpeg command arguments"), AdvancedDisplay)
	TArray<FString> FFmpegCmdArgs;

private:
	bool ValidateOutputDirectory();
	bool ValidateFFmpegExe();
	bool ValidateResolution();

	FIntPoint CurrentResolution;
};
