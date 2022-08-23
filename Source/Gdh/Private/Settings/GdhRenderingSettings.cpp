// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhRenderingSettings.h"
#include "Gdh.h"
// Engine Headers
// #include "MoviePipelineAntiAliasingSetting.h"
// #include "MoviePipelineCameraSetting.h"
// #include "MoviePipelineCommandLineEncoderSettings.h"
// #include "MoviePipelineConsoleVariableSetting.h"
// #include "MoviePipelineDeferredPasses.h"
// #include "MoviePipelineHighResSetting.h"
#include "MoviePipelineImageSequenceOutput.h"
// #include "MoviePipelineWidgetRenderSetting.h"
// #include "MovieSceneTimeHelpers.h"
// #include "Kismet/KismetStringLibrary.h"

UGdhRenderingSettings::UGdhRenderingSettings()
{

}

FName UGdhRenderingSettings::GetContainerName() const
{
	return FName{"Project"};
}

FName UGdhRenderingSettings::GetCategoryName() const
{
	return GdhConstants::ModuleFullName;
}

FName UGdhRenderingSettings::GetSectionName() const
{
	return FName{"Rendering Settings"};
}

FString UGdhRenderingSettings::GetDesc()
{
	return TEXT("GamedevHelper Rendering Settings");
}

#if WITH_EDITOR
void UGdhRenderingSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	switch (ResolutionPreset)
	{
		case EGdhResolutionPreset::Res360P:
			CurrentResolution = GdhConstants::Resolution360P;
			break;
		case EGdhResolutionPreset::Res480P:
			CurrentResolution = GdhConstants::Resolution480P;
			break;
		case EGdhResolutionPreset::Res720P:
			CurrentResolution = GdhConstants::Resolution720P;
			break;
		case EGdhResolutionPreset::Res1080P:
			CurrentResolution = GdhConstants::Resolution1080P;
			break;
		case EGdhResolutionPreset::Res1440P:
			CurrentResolution = GdhConstants::Resolution1440P;
			break;
		case EGdhResolutionPreset::Res2160P:
			CurrentResolution = GdhConstants::Resolution2160P;
			break;
		case EGdhResolutionPreset::ResCustom:
			CurrentResolution = CustomResolution;
			break;
		default:
			CurrentResolution = GdhConstants::Resolution1080P;
	}

	if (FPaths::FileExists(FFmpegExe.FilePath))
	{
		FFmpegExe.FilePath = FPaths::ConvertRelativePathToFull(FFmpegExe.FilePath);
	}

	SaveConfig();
}
#endif

bool UGdhRenderingSettings::IsValidSettings() const
{
	if (OutputDirectory.Path.IsEmpty()) return false;
	if (!FPaths::DirectoryExists(OutputDirectory.Path)) return false;
	if (FFmpegExe.FilePath.IsEmpty()) return false;
	if (FFmpegExe.FilePath.ToLower().Equals(TEXT("ffmpeg.exe"))) return true;
	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(FFmpegExe.FilePath))) return false;
	if (CurrentResolution.X % 2 != 0 || CurrentResolution.Y % 2 != 0) return false;
	
	return true;
}
//
// void UGdhRenderingSettings::Validate()
// {
// 	if (!ValidateOutputDirectory())
// 	{
// 		return;
// 	}
//
// 	if (!ValidateFFmpegExe())
// 	{
// 		return;
// 	}
//
// 	if (!ValidateResolution())
// 	{
// 		return;
// 	}
//
// 	if (!ValidateMovieRenderSettings())
// 	{
// 		return;
// 	}
//
// 	MarkValid();
// }

// void UGdhRenderingSettings::LoadDefaultSettings()
// {
// 	Super::LoadDefaultSettings();
// }

FIntPoint UGdhRenderingSettings::GetResolution() const
{
	return CurrentResolution;
}

FString UGdhRenderingSettings::GetResolutionFolderName() const
{
	switch (ResolutionPreset)
	{
		case EGdhResolutionPreset::Res360P:
			return TEXT("360p");
		case EGdhResolutionPreset::Res480P:
			return TEXT("480p");
		case EGdhResolutionPreset::Res720P:
			return TEXT("720p");
		case EGdhResolutionPreset::Res1080P:
			return TEXT("1080p");
		case EGdhResolutionPreset::Res1440P:
			return TEXT("1440p");
		case EGdhResolutionPreset::Res2160P:
			return TEXT("2160p");
		case EGdhResolutionPreset::ResCustom:
			return TEXT("custom_resolution");
		default:
			return TEXT("1080p");
	}
}

UClass* UGdhRenderingSettings::GetImageClass() const
{
	switch (ImageFormat)
	{
		case EGdhImageFormat::Png:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
		case EGdhImageFormat::Jpg:
			return UMoviePipelineImageSequenceOutput_JPG::StaticClass();
		case EGdhImageFormat::Bmp:
			return UMoviePipelineImageSequenceOutput_BMP::StaticClass();
		default:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
	}
}

FString UGdhRenderingSettings::GetImageExtension(const bool IncludeDot) const
{
	switch (ImageFormat)
	{
		case EGdhImageFormat::Png:
			return IncludeDot ? TEXT(".png") : TEXT("png");
		case EGdhImageFormat::Jpg:
			return IncludeDot ? TEXT(".jpg") : TEXT("jpg");
		case EGdhImageFormat::Bmp:
			return IncludeDot ? TEXT(".bmp") : TEXT("bmp");
		default:
			return IncludeDot ? TEXT(".png") : TEXT("png");
	}
}

FString UGdhRenderingSettings::GetVideoExtension(const bool IncludeDot) const
{
	switch (VideoFormat)
	{
		case EGdhVideoFormat::Mp4:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
		case EGdhVideoFormat::Mkv:
			return IncludeDot ? TEXT(".mkv") : TEXT("mkv");
		case EGdhVideoFormat::Avi:
			return IncludeDot ? TEXT(".avi") : TEXT("avi");
		default:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
	}
}

// UMoviePipelineMasterConfig* UGdhRenderingSettings::GetMasterConfig() const
// {
// 	UMoviePipelineMasterConfig* Config = NewObject<UMoviePipelineMasterConfig>();
// 	// if (!Config) return nullptr;
// 	//
// 	// Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
// 	// Config->FindOrAddSettingByClass(GetImageClass());
// 	//
// 	// UMoviePipelineCommandLineEncoderSettings* EncoderSettings = GetMutableDefault<UMoviePipelineCommandLineEncoderSettings>();
// 	// EncoderSettings->OutputFileExtension = GetVideoExtension();
// 	// EncoderSettings->PostEditChange();
// 	//
// 	// if (bSettingsAAEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineAntiAliasingSetting> AntiAliasingSetting = Config->FindOrAddSettingByClass(UMoviePipelineAntiAliasingSetting::StaticClass()))
// 	// 	{
// 	// 		AntiAliasingSetting->SpatialSampleCount = SpatialSampleCount;
// 	// 		AntiAliasingSetting->TemporalSampleCount = TemporalSampleCount;
// 	// 		AntiAliasingSetting->bOverrideAntiAliasing = bOverrideAntiAliasing;
// 	// 		AntiAliasingSetting->AntiAliasingMethod = AntiAliasingMethod;
// 	// 		AntiAliasingSetting->RenderWarmUpCount = RenderWarmUpCount;
// 	// 		AntiAliasingSetting->bUseCameraCutForWarmUp = bUseCameraCutForWarmUp;
// 	// 		AntiAliasingSetting->EngineWarmUpCount = EngineWarmUpCount;
// 	// 		AntiAliasingSetting->bRenderWarmUpFrames = bRenderWarmUpFrames;
// 	// 	}
// 	// }
// 	//
// 	// if (bSettingsConsoleVariablesEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineConsoleVariableSetting> ConsoleVariableSetting = Config->FindOrAddSettingByClass(UMoviePipelineConsoleVariableSetting::StaticClass()))
// 	// 	{
// 	// 		ConsoleVariableSetting->ConsoleVariables = ConsoleVariables;
// 	// 		ConsoleVariableSetting->StartConsoleCommands = StartConsoleCommands;
// 	// 		ConsoleVariableSetting->EndConsoleCommands = EndConsoleCommands;
// 	// 	}
// 	// }
// 	//
// 	// if (bSettingsGameOverrideEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineGameOverrideSetting> GameOverrideSetting = Config->FindOrAddSettingByClass(UMoviePipelineGameOverrideSetting::StaticClass()))
// 	// 	{
// 	// 		GameOverrideSetting->GameModeOverride = GameModeOverride;
// 	// 		GameOverrideSetting->bCinematicQualitySettings = bCinematicQualitySettings;
// 	// 		GameOverrideSetting->TextureStreaming = TextureStreaming;
// 	// 		GameOverrideSetting->bUseLODZero = bUseLODZero;
// 	// 		GameOverrideSetting->bDisableHLODs = bDisableHLODs;
// 	// 		GameOverrideSetting->bUseHighQualityShadows = bUseHighQualityShadows;
// 	// 		GameOverrideSetting->ShadowDistanceScale = ShadowDistanceScale;
// 	// 		GameOverrideSetting->ShadowRadiusThreshold = ShadowRadiusThreshold;
// 	// 		GameOverrideSetting->bOverrideViewDistanceScale = bOverrideViewDistanceScale;
// 	// 		GameOverrideSetting->ViewDistanceScale = ViewDistanceScale;
// 	// 		GameOverrideSetting->bFlushGrassStreaming = bFlushGrassStreaming;
// 	// 	}
// 	// }
// 	//
// 	// if (bSettingsUIEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineWidgetRenderer> WidgetRenderer = Config->FindOrAddSettingByClass(UMoviePipelineWidgetRenderer::StaticClass()))
// 	// 	{
// 	// 		WidgetRenderer->bCompositeOntoFinalImage = bCompositeOntoFinalImage;
// 	// 	}
// 	// }
// 	//
// 	// if (bSettingsBurnInEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineBurnInSetting> BurnInSetting = Config->FindOrAddSettingByClass(UMoviePipelineBurnInSetting::StaticClass()))
// 	// 	{
// 	// 		BurnInSetting->BurnInClass = BurnInClass;
// 	// 		BurnInSetting->bCompositeOntoFinalImage = bBurnInCompositeOntoFinalImage;
// 	// 	}
// 	// }
// 	//
// 	// if (bSettingsCameraEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineCameraSetting> CameraSetting = Config->FindOrAddSettingByClass(UMoviePipelineCameraSetting::StaticClass()))
// 	// 	{
// 	// 		CameraSetting->OverscanPercentage = OverscanPercentage;
// 	// 		CameraSetting->ShutterTiming = ShutterTiming;
// 	// 	}
// 	// }
// 	//
// 	// if (bSettingsHighResEnabled)
// 	// {
// 	// 	if (const TSoftObjectPtr<UMoviePipelineHighResSetting> HighResSetting = Config->FindOrAddSettingByClass(UMoviePipelineHighResSetting::StaticClass()))
// 	// 	{
// 	// 		HighResSetting->TileCount = TileCount;
// 	// 		HighResSetting->TextureSharpnessBias = TextureSharpnessBias;
// 	// 		HighResSetting->OverlapRatio = OverlapRatio;
// 	// 		HighResSetting->bOverrideSubSurfaceScattering = bOverrideSubSurfaceScattering;
// 	// 		HighResSetting->BurleySampleCount = BurleySampleCount;
// 	// 	}
// 	// }
//
// 	return Config;
// }

// UMoviePipelineMasterConfig* UGdhRenderingSettings::GetMasterConfig(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue) const
// {
// 	if (!LevelSequence) return nullptr;
// 	if (!LevelSequence->MovieScene) return nullptr;
//
// 	UMoviePipelineMasterConfig* Config = GetMasterConfig();
// 	if (!Config) return nullptr;
//
// 	const TSoftObjectPtr<UMoviePipelineOutputSetting> OutputSetting = Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass());
// 	if (!OutputSetting) return nullptr;
//
// 	const int32 FrameStart = ConvertFrameTime(
// 		UE::MovieScene::DiscreteInclusiveLower(LevelSequence->MovieScene->GetPlaybackRange()),
// 		LevelSequence->MovieScene->GetTickResolution(),
// 		LevelSequence->MovieScene->GetDisplayRate()
// 	).FloorToFrame().Value;
// 	const int32 FrameEnd = ConvertFrameTime(
// 		UE::MovieScene::DiscreteExclusiveUpper(LevelSequence->MovieScene->GetPlaybackRange()),
// 		LevelSequence->MovieScene->GetTickResolution(),
// 		LevelSequence->MovieScene->GetDisplayRate()
// 	).FloorToFrame().Value;
//
// 	OutputSetting->FileNameFormat = TEXT("{sequence_name}_{frame_number_rel}");
// 	OutputSetting->OutputResolution = CurrentResolution;
// 	OutputSetting->bUseCustomFrameRate = true;
// 	OutputSetting->OutputFrameRate = Framerate;
// 	OutputSetting->bOverrideExistingOutput = true;
// 	OutputSetting->ZeroPadFrameNumbers = 4;
// 	OutputSetting->FrameNumberOffset = 0;
// 	OutputSetting->HandleFrameCount = 0;
// 	OutputSetting->OutputFrameStep = 1;
// 	OutputSetting->bUseCustomPlaybackRange = true;
// 	OutputSetting->CustomStartFrame = FrameStart;
// 	OutputSetting->CustomEndFrame = FrameEnd;
//
//
// 	// const FString ImageOutputDir = GetImageOutputDir(LevelSequence, MoviePipelineQueue);
// 	// if (!FPaths::DirectoryExists(ImageOutputDir))
// 	// {
// 	// 	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
// 	// 	PlatformFile.CreateDirectoryTree(*ImageOutputDir);
// 	// }
// 	//
// 	// OutputSetting->OutputDirectory.Path = ImageOutputDir;
//
// 	return Config;
// }
//
// bool UGdhRenderingSettings::ValidateOutputDirectory()
// {
// 	// if (OutputDirectory.Path.IsEmpty())
// 	// {
// 	// 	SetErrorMsg(TEXT("Output directory not specified"));
// 	// 	MarkInvalid();
// 	// 	return false;
// 	// }
// 	//
// 	// if (!FPaths::DirectoryExists(OutputDirectory.Path))
// 	// {
// 	// 	SetErrorMsg(TEXT("Output directory does not exist"));
// 	// 	MarkInvalid();
// 	// 	return false;
// 	// }
//
// 	return true;
// }
//
// bool UGdhRenderingSettings::ValidateFFmpegExe()
// {
// 	// if (FFmpegExe.FilePath.ToLower().Equals(TEXT("ffmpeg.exe")))
// 	// {
// 	// 	return true;
// 	// }
// 	//
// 	// if (FFmpegExe.FilePath.IsEmpty())
// 	// {
// 	// 	SetErrorMsg(TEXT("FFmpeg exe not specified"));
// 	// 	MarkInvalid();
// 	// 	return false;
// 	// }
// 	//
// 	// if (!FPaths::FileExists(FFmpegExe.FilePath))
// 	// {
// 	// 	SetErrorMsg(TEXT("FFmpeg exe not found"));
// 	// 	MarkInvalid();
// 	// 	return false;
// 	// }
//
// 	return true;
// }
//
// bool UGdhRenderingSettings::ValidateResolution()
// {
// 	// if (CurrentResolution.X % 2 != 0 || CurrentResolution.Y % 2 != 0)
// 	// {
// 	// 	SetErrorMsg(TEXT("Resolution must have dimensions divisible by 2"));
// 	// 	MarkInvalid();
// 	// 	return false;
// 	// }
//
// 	return true;
// }
//
// bool UGdhRenderingSettings::ValidateMovieRenderSettings()
// {
// 	const UMoviePipelineMasterConfig* Config = GetMasterConfig();
// 	if (!Config) return false;
//
// 	for (const auto& Setting : Config->GetAllSettings())
// 	{
// 		if (!IsValidJobSetting(Setting))
// 		{
// 			return false;
// 		}
// 	}
//
// 	return true;
// }
//
// bool UGdhRenderingSettings::IsValidJobSetting(UMoviePipelineSetting* Setting)
// {
// 	if (!Setting) return false;
//
// 	Setting->ValidateState();
//
// 	if (Setting->GetValidationState() == EMoviePipelineValidationState::Errors || Setting->GetValidationState() == EMoviePipelineValidationState::Warnings)
// 	{
// 		const TArray<FText> ValidationResults = Setting->GetValidationResults();
//
// 		FString ValidateErrorText;
//
// 		for (const auto& ValidationResult : ValidationResults)
// 		{
// 			ValidateErrorText.Append(ValidationResult.ToString() + TEXT("\n"));
// 		}
//
// 		// SetErrorMsg(ValidateErrorText);
// 		// MarkInvalid();
// 		return false;
// 	}
//
// 	return true;
// }
