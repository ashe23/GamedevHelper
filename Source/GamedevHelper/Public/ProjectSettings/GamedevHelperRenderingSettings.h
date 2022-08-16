// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GamedevHelperTypes.h"
#include "MoviePipelineBurnInSetting.h"
#include "MoviePipelineGameOverrideSetting.h"
#include "MovieRenderPipelineDataTypes.h"
#include "GamedevHelperRenderingSettings.generated.h"

UCLASS(BlueprintType, Config = EditorPerProjectUserSettings, meta=(DisplayName="Rendering Settings"))
class GAMEDEVHELPER_API UGamedevHelperRenderingSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGamedevHelperRenderingSettings();

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
	virtual FString GetDesc() override;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetErrorMsg() const;
	
	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	void Validate();
	
	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	bool IsValid() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	UClass* GetImageClass() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FIntPoint GetResolution() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetResolutionFolderName() const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetImageOutputDirectory(TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue, TSoftObjectPtr<ULevelSequence> LevelSequence) const;
	
	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetVideoOutputDirectory(TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue) const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetImageExtension(const bool IncludeDot = false) const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	FString GetVideoExtension(const bool IncludeDot = false) const;

	UFUNCTION(BlueprintCallable, Category="Rendering Settings")
	UMoviePipelineMasterConfig* GetMasterConfig();

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category="General")
	FDirectoryPath OutputDirectory;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category="General")
	TArray<FString> FFmpegExtraCommands;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Format", DisplayName = "Resolution", meta = (ToolTip = "Rendering resolution presets. By Default is 1080p"))
	EGamedevHelperResolutionPreset ResolutionPreset = EGamedevHelperResolutionPreset::Res1080P;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Format", DisplayName = "Custom Resolution",
		meta = (
			ToolTip = "Max resolution: 4096",
			EditCondition = "ResolutionPreset == EGamedevHelperResolutionPreset::ResCustom",
			UIMin="1", UIMax="4096",
			ClampMin="1", ClampMax="4096"))
	FIntPoint CustomResolution = GamedevHelperConstants::Resolution1080P;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Format", DisplayName = "Framerate", meta = (ToolTip = "Output framerate. Default is 30 fps"))
	FFrameRate Framerate = GamedevHelperConstants::DefaultFrameRate;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Format", DisplayName = "Image Format", meta = (ToolTip = "Output image format. Default is .png"))
	EGamedevHelperImageFormat ImageFormat = EGamedevHelperImageFormat::Png;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Format", DisplayName = "Video Format", meta = (ToolTip = "Output video format. Default is .mp4"))
	EGamedevHelperVideoFormat VideoFormat = EGamedevHelperVideoFormat::Mp4;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", DisplayName="Use AA settings")
	bool bSettingsAAEnabled = false;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (UIMin = 1, ClampMin = 1, EditCondition="bSettingsAAEnabled"))
	int32 SpatialSampleCount = 1;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (UIMin = 1, ClampMin = 1, EditCondition="bSettingsAAEnabled"))
	int32 TemporalSampleCount = 1;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (EditCondition="bSettingsAAEnabled"))
	bool bOverrideAntiAliasing = false;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (EditCondition="bOverrideAntiAliasing && bSettingsAAEnabled"))
	TEnumAsByte<EAntiAliasingMethod> AntiAliasingMethod = AAM_None;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (UIMin = 0, ClampMin = 0, EditCondition="bSettingsAAEnabled"), AdvancedDisplay)
	int32 RenderWarmUpCount = 32;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (EditCondition="bSettingsAAEnabled"))
	bool bUseCameraCutForWarmUp = false;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (UIMin = 0, ClampMin = 0, EditCondition="!bUseCameraCutForWarmUp && bSettingsAAEnabled"), AdvancedDisplay)
	int32 EngineWarmUpCount = 0;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "AntiAliasing", meta = (EditCondition="bSettingsAAEnabled"))
	bool bRenderWarmUpFrames = false;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "ConsoleVariables", DisplayName="Use Console Variables Settings")
	bool bSettingsConsoleVariablesEnabled = false;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "ConsoleVariables", meta = (EditCondition="bSettingsConsoleVariablesEnabled"))
	TMap<FString, float> ConsoleVariables;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "ConsoleVariables", meta = (EditCondition="bSettingsConsoleVariablesEnabled"))
	TArray<FString> StartConsoleCommands;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "ConsoleVariables", meta = (EditCondition="bSettingsConsoleVariablesEnabled"))
	TArray<FString> EndConsoleCommands;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride", DisplayName="Use GameOverride Settings")
	bool bSettingsGameOverrideEnabled = false;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Game", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	TSubclassOf<AGameModeBase> GameModeOverride = AMoviePipelineGameMode::StaticClass();

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Game", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	bool bCinematicQualitySettings = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	EMoviePipelineTextureStreamingMethod TextureStreaming = EMoviePipelineTextureStreamingMethod::Disabled;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	bool bUseLODZero = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	bool bDisableHLODs = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	bool bUseHighQualityShadows = true;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bUseHighQualityShadows && bSettingsGameOverrideEnabled", ClampMin=0.1, UIMin=0.1, UIMax=10))
	int32 ShadowDistanceScale = 10;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bUseHighQualityShadows && bSettingsGameOverrideEnabled", UIMin=0.001, ClampMin=0.001))
	float ShadowRadiusThreshold = 0.001f;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	bool bOverrideViewDistanceScale = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bOverrideViewDistanceScale && bSettingsGameOverrideEnabled"))
	int32 ViewDistanceScale = 50;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "GameOverride|Rendering", meta = (EditCondition="bSettingsGameOverrideEnabled"))
	bool bFlushGrassStreaming = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "UI Renderer", DisplayName="Use UI Settings")
	bool bSettingsUIEnabled = false;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "UI Renderer", meta = (MetaClass = "MoviePipelineBurnInWidget", EditCondition="bSettingsUIEnabled"))
	bool bCompositeOntoFinalImage = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "BurnIn", DisplayName="Use BurnIn Settings")
	bool bSettingsBurnInEnabled = false;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "BurnIn", meta = (MetaClass="MoviePipelineBurnInWidget", EditCondition="bSettingsBurnInEnabled"))
	FSoftClassPath BurnInClass = UMoviePipelineBurnInSetting::DefaultBurnInWidgetAsset;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "BurnIn", meta = (MetaClass="MoviePipelineBurnInWidget", EditCondition="bSettingsBurnInEnabled"))
	bool bBurnInCompositeOntoFinalImage = true;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Camera Settings", DisplayName="Use Camera Settings")
	bool bSettingsCameraEnabled = false;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Camera Settings", meta = (EditCondition="bSettingsCameraEnabled"))
	EMoviePipelineShutterTiming ShutterTiming = EMoviePipelineShutterTiming::FrameCenter;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Camera Settings", meta = (EditCondition="bSettingsCameraEnabled",UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float OverscanPercentage = 0.0f;
private:
	bool IsValidateJobSetting(TSoftObjectPtr<UMoviePipelineSetting> Setting);

	FIntPoint CurrentResolution = GamedevHelperConstants::Resolution1080P;
	FString ErrorMsg;
};
