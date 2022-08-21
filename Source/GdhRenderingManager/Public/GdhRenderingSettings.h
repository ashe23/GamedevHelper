// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MoviePipelineBurnInSetting.h"
#include "MoviePipelineGameOverrideSetting.h"
#include "MovieRenderPipelineDataTypes.h"
#include "GdhRenderingManagerTypes.h"
#include "GdhCoreDeveloperSettings.h"
#include "GdhRenderingManager.h"
#include "GdhRenderingSettings.generated.h"

class UMoviePipelineQueue;

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

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetVideoExtension(const bool IncludeDot = false) const;

	UMoviePipelineMasterConfig* GetMasterConfig() const;
	UMoviePipelineMasterConfig* GetMasterConfig(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;
	FString GetImageOutputDir(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;
	FString GetVideoOutputDir(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;
	FString GetEncodeCommand(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue = nullptr) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="Output directory for rendered images and videos"))
	FDirectoryPath OutputDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="General", meta=(ToolTip="FFmpeg exe path", FilePathFilter="exe"))
	FFilePath FFmpegExe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered image format"))
	EGdhImageFormat ImageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered image resolutions"))
	EGdhResolutionPreset ResolutionPreset = GdhRenderingManagerConstants::DefaultResolutionPreset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", DisplayName="Custom Resolution",
		meta=(
			ToolTip = "Max resolution: 4096",
			EditCondition = "ResolutionPreset == EGdhResolutionPreset::ResCustom",
			UIMin="1", UIMax="4096",
			ClampMin="1", ClampMax="4096"))
	FIntPoint CustomResolution = GdhRenderingManagerConstants::DefaultResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Rendered video format"))
	EGdhVideoFormat VideoFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings")
	FFrameRate Framerate = GdhRenderingManagerConstants::DefaultFrameRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="OutputSettings", meta=(ToolTip="Additional ffmpeg command arguments. Used when encoding video"), AdvancedDisplay)
	TArray<FString> FFmpegCmdArgs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", DisplayName="Use AA settings")
	bool bSettingsAAEnabled = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(UIMin = 1, ClampMin = 1, EditCondition="bSettingsAAEnabled"))
	int32 SpatialSampleCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(UIMin = 1, ClampMin = 1, EditCondition="bSettingsAAEnabled"))
	int32 TemporalSampleCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(EditCondition="bSettingsAAEnabled"))
	bool bOverrideAntiAliasing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(EditCondition="bOverrideAntiAliasing && bSettingsAAEnabled"))
	TEnumAsByte<EAntiAliasingMethod> AntiAliasingMethod = AAM_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(UIMin = 0, ClampMin = 0, EditCondition="bSettingsAAEnabled"), AdvancedDisplay)
	int32 RenderWarmUpCount = 32;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(EditCondition="bSettingsAAEnabled"))
	bool bUseCameraCutForWarmUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(UIMin = 0, ClampMin = 0, EditCondition="!bUseCameraCutForWarmUp && bSettingsAAEnabled"), AdvancedDisplay)
	int32 EngineWarmUpCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|AntiAliasing", meta=(EditCondition="bSettingsAAEnabled"))
	bool bRenderWarmUpFrames = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|ConsoleVariables", DisplayName="Use Console Variables Settings")
	bool bSettingsConsoleVariablesEnabled = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|ConsoleVariables", meta=(EditCondition="bSettingsConsoleVariablesEnabled"))
	TMap<FString, float> ConsoleVariables;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|ConsoleVariables", meta=(EditCondition="bSettingsConsoleVariablesEnabled"))
	TArray<FString> StartConsoleCommands;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|ConsoleVariables", meta=(EditCondition="bSettingsConsoleVariablesEnabled"))
	TArray<FString> EndConsoleCommands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride", DisplayName="Use GameOverride Settings")
	bool bSettingsGameOverrideEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Game", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	TSubclassOf<AGameModeBase> GameModeOverride = AMoviePipelineGameMode::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Game", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	bool bCinematicQualitySettings = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	EMoviePipelineTextureStreamingMethod TextureStreaming = EMoviePipelineTextureStreamingMethod::Disabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	bool bUseLODZero = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	bool bDisableHLODs = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	bool bUseHighQualityShadows = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bUseHighQualityShadows && bSettingsGameOverrideEnabled", ClampMin=0.1, UIMin=0.1, UIMax=10))
	int32 ShadowDistanceScale = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bUseHighQualityShadows && bSettingsGameOverrideEnabled", UIMin=0.001, ClampMin=0.001))
	float ShadowRadiusThreshold = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	bool bOverrideViewDistanceScale = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bOverrideViewDistanceScale && bSettingsGameOverrideEnabled"))
	int32 ViewDistanceScale = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|GameOverride|Rendering", meta=(EditCondition="bSettingsGameOverrideEnabled"))
	bool bFlushGrassStreaming = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|UI", DisplayName="Use UI Settings")
	bool bSettingsUIEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|UI", meta=(MetaClass="MoviePipelineBurnInWidget", EditCondition="bSettingsUIEnabled"))
	bool bCompositeOntoFinalImage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|BurnIn", DisplayName="Use BurnIn Settings")
	bool bSettingsBurnInEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|BurnIn", meta=(MetaClass="MoviePipelineBurnInWidget", EditCondition="bSettingsBurnInEnabled"))
	FSoftClassPath BurnInClass = UMoviePipelineBurnInSetting::DefaultBurnInWidgetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|BurnIn", meta=(MetaClass="MoviePipelineBurnInWidget", EditCondition="bSettingsBurnInEnabled"))
	bool bBurnInCompositeOntoFinalImage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|Camera Settings", DisplayName="Use Camera Settings")
	bool bSettingsCameraEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|Camera Settings", meta=(EditCondition="bSettingsCameraEnabled"))
	EMoviePipelineShutterTiming ShutterTiming = EMoviePipelineShutterTiming::FrameCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|Camera Settings", meta=(EditCondition="bSettingsCameraEnabled",UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float OverscanPercentage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|HighRes", DisplayName="Use HighRes Settings")
	bool bSettingsHighResEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|HighRes", meta=(UIMin="1", ClampMin="1", UIMax="16", EditCondition="bSettingsHighResEnabled"))
	int32 TileCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|HighRes", meta=(UIMin="-1", UIMax="0", EditCondition="bSettingsHighResEnabled"))
	float TextureSharpnessBias = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|HighRes", meta=(UIMin="0", ClampMin="0", UIMax="0.5", ClampMax="1", EditCondition="bSettingsHighResEnabled"))
	float OverlapRatio = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|HighRes", meta=(EditCondition="bSettingsHighResEnabled"))
	bool bOverrideSubSurfaceScattering = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="MovieRenderSettings|HighRes", meta=(UIMin="64", ClampMin="0", UIMax="1024", EditCondition="bOverrideSubSurfaceScattering && bSettingsHighResEnabled"))
	int32 BurleySampleCount = 64;

private:
	bool ValidateOutputDirectory();
	bool ValidateFFmpegExe();
	bool ValidateResolution();
	bool ValidateMovieRenderSettings();
	bool IsValidJobSetting(UMoviePipelineSetting* Setting);

	FIntPoint CurrentResolution = GdhRenderingManagerConstants::DefaultResolution;
};
