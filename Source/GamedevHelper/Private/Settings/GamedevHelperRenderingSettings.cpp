// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GamedevHelperRenderingSettings.h"
#include "GamedevHelper.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineCameraSetting.h"
#include "MoviePipelineCommandLineEncoder.h"
#include "MoviePipelineCommandLineEncoderSettings.h"
#include "MoviePipelineConsoleVariableSetting.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineImageSequenceOutput.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MoviePipelineWidgetRenderSetting.h"

UGamedevHelperRenderingSettings::UGamedevHelperRenderingSettings()
{
	FFmpegExtraCommands.Add(TEXT("-vcodec libx264"));
	FFmpegExtraCommands.Add(TEXT("-crf 10"));
	FFmpegExtraCommands.Add(TEXT("-pix_fmt yuv420p"));
	
	OnSettingChanged().AddLambda([&](const UObject* Object, const FPropertyChangedEvent& PropertyChangedEvent)
	{
		if (!PropertyChangedEvent.MemberProperty) return;
		const FString PropertyName = PropertyChangedEvent.MemberProperty->GetName();

		if (PropertyName.Equals(TEXT("ResolutionPreset")) || PropertyName.Equals(TEXT("CustomResolution")))
		{
			switch (ResolutionPreset)
			{
				case EGamedevHelperResolutionPreset::Res360P:
					CurrentResolution = GamedevHelperConstants::Resolution360P;
					break;
				case EGamedevHelperResolutionPreset::Res480P:
					CurrentResolution = GamedevHelperConstants::Resolution480P;
					break;
				case EGamedevHelperResolutionPreset::Res720P:
					CurrentResolution = GamedevHelperConstants::Resolution720P;
					break;
				case EGamedevHelperResolutionPreset::Res1080P:
					CurrentResolution = GamedevHelperConstants::Resolution1080P;
					break;
				case EGamedevHelperResolutionPreset::Res2160P:
					CurrentResolution = GamedevHelperConstants::Resolution2160P;
					break;
				case EGamedevHelperResolutionPreset::ResCustom:
					CurrentResolution = CustomResolution;
					break;
				default:
					CurrentResolution = GamedevHelperConstants::Resolution1080P;
			}
		}

		Validate();
	});
}

FName UGamedevHelperRenderingSettings::GetContainerName() const
{
	return FName{"Project"};
}

FName UGamedevHelperRenderingSettings::GetCategoryName() const
{
	return FName{"GamedevHelper"};
}

FName UGamedevHelperRenderingSettings::GetSectionName() const
{
	return FName{"RenderingSettings"};
}

FString UGamedevHelperRenderingSettings::GetDesc()
{
	return TEXT("GamedevHelper Rendering Settings");
}

FString UGamedevHelperRenderingSettings::GetErrorMsg() const
{
	return ErrorMsg;
}

void UGamedevHelperRenderingSettings::Validate()
{
	if (OutputDirectory.Path.IsEmpty())
	{
		ErrorMsg = TEXT("Error: Output directory not specified");
		return;
	}
	if (!FPaths::DirectoryExists(OutputDirectory.Path))
	{
		ErrorMsg = TEXT("Error: Output directory does not exist");
		return;
	}

	if (CurrentResolution.X % 2 != 0 || CurrentResolution.Y % 2 != 0)
	{
		ErrorMsg = TEXT("Error: Resolution must be divisible by 2");
		return;
	}

	bool bSettingsValid = true;
	const TSoftObjectPtr<UMoviePipelineMasterConfig> Config = GetMasterConfig();
	for (const auto& Setting : Config->GetAllSettings())
	{
		if (!IsValidJobSetting(Setting))
		{
			bSettingsValid = false;
		}
	}

	if (bSettingsValid)
	{
		ErrorMsg.Reset();
	}
}

bool UGamedevHelperRenderingSettings::IsValid() const
{
	return ErrorMsg.IsEmpty();
}

UClass* UGamedevHelperRenderingSettings::GetImageClass() const
{
	switch (ImageFormat)
	{
		case EGamedevHelperImageFormat::Png:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
		case EGamedevHelperImageFormat::Jpg:
			return UMoviePipelineImageSequenceOutput_JPG::StaticClass();
		case EGamedevHelperImageFormat::Bmp:
			return UMoviePipelineImageSequenceOutput_BMP::StaticClass();
		default:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
	}
}

FIntPoint UGamedevHelperRenderingSettings::GetResolution() const
{
	return CurrentResolution;
}

FString UGamedevHelperRenderingSettings::GetResolutionFolderName() const
{
	switch (ResolutionPreset)
	{
		case EGamedevHelperResolutionPreset::Res360P:
			return TEXT("360p");
		case EGamedevHelperResolutionPreset::Res480P:
			return TEXT("480p");
		case EGamedevHelperResolutionPreset::Res720P:
			return TEXT("720p");
		case EGamedevHelperResolutionPreset::Res1080P:
			return TEXT("1080p");
		case EGamedevHelperResolutionPreset::Res2160P:
			return TEXT("2160p");
		case EGamedevHelperResolutionPreset::ResCustom:
			return TEXT("custom_resolution");
		default:
			return TEXT("1080p");
	}
}

FString UGamedevHelperRenderingSettings::GetImageOutputDirectory(TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue, TSoftObjectPtr<ULevelSequence> LevelSequence) const
{
	if (!MoviePipelineQueue || !LevelSequence) return TEXT("");

	// {output_directory}/image/{resolution_folder}/{queue_name}/{level_sequence_name}

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/image/%s/%s/%s"),
			*OutputDirectory.Path,
			*GetResolutionFolderName(),
			*MoviePipelineQueue.GetAssetName(),
			*LevelSequence.GetAssetName()
		)
	);
}

FString UGamedevHelperRenderingSettings::GetVideoOutputDirectory(TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue) const
{
	if (!MoviePipelineQueue) return TEXT("");

	// {output_directory}/video/{resolution_folder}/{queue_name}/

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/video/%s/%s"),
			*OutputDirectory.Path,
			*GetResolutionFolderName(),
			*MoviePipelineQueue.GetAssetName()
		)
	);
}

FString UGamedevHelperRenderingSettings::GetImageExtension(const bool IncludeDot) const
{
	switch (ImageFormat)
	{
		case EGamedevHelperImageFormat::Png:
			return IncludeDot ? TEXT(".png") : TEXT("png");
		case EGamedevHelperImageFormat::Jpg:
			return IncludeDot ? TEXT(".jpeg") : TEXT("jpeg");
		case EGamedevHelperImageFormat::Bmp:
			return IncludeDot ? TEXT(".bmp") : TEXT("bmp");
		default:
			return IncludeDot ? TEXT(".png") : TEXT("png");
	}
}

FString UGamedevHelperRenderingSettings::GetVideoExtension(const bool IncludeDot) const
{
	switch (VideoFormat)
	{
		case EGamedevHelperVideoFormat::Mp4:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
		case EGamedevHelperVideoFormat::Mkv:
			return IncludeDot ? TEXT(".mkv") : TEXT("mkv");
		case EGamedevHelperVideoFormat::Avi:
			return IncludeDot ? TEXT(".avi") : TEXT("avi");
		default:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
	}
}

UMoviePipelineMasterConfig* UGamedevHelperRenderingSettings::GetMasterConfig()
{
	UMoviePipelineMasterConfig* Config = NewObject<UMoviePipelineMasterConfig>();
	if (!Config) return nullptr;
	
	Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
	Config->FindOrAddSettingByClass(GetImageClass());
	
	UMoviePipelineCommandLineEncoderSettings* EncoderSettings = GetMutableDefault<UMoviePipelineCommandLineEncoderSettings>();
	EncoderSettings->OutputFileExtension = GetVideoExtension();
	EncoderSettings->PostEditChange();
	
	if (bSettingsAAEnabled)
	{
		const TSoftObjectPtr<UMoviePipelineAntiAliasingSetting> AntiAliasingSetting = Config->FindOrAddSettingByClass(UMoviePipelineAntiAliasingSetting::StaticClass());
		if (AntiAliasingSetting)
		{
			AntiAliasingSetting->SpatialSampleCount = SpatialSampleCount;
			AntiAliasingSetting->TemporalSampleCount = TemporalSampleCount;
			AntiAliasingSetting->bOverrideAntiAliasing = bOverrideAntiAliasing;
			AntiAliasingSetting->AntiAliasingMethod = AntiAliasingMethod;
			AntiAliasingSetting->RenderWarmUpCount = RenderWarmUpCount;
			AntiAliasingSetting->bUseCameraCutForWarmUp = bUseCameraCutForWarmUp;
			AntiAliasingSetting->EngineWarmUpCount = EngineWarmUpCount;
			AntiAliasingSetting->bRenderWarmUpFrames = bRenderWarmUpFrames;
		}

	}

	if (bSettingsConsoleVariablesEnabled)
	{
		const TSoftObjectPtr<UMoviePipelineConsoleVariableSetting> ConsoleVariableSetting = Config->FindOrAddSettingByClass(UMoviePipelineConsoleVariableSetting::StaticClass());
		if (ConsoleVariableSetting)
		{
			ConsoleVariableSetting->ConsoleVariables = ConsoleVariables;
			ConsoleVariableSetting->StartConsoleCommands = StartConsoleCommands;
			ConsoleVariableSetting->EndConsoleCommands = EndConsoleCommands;
		}
	}

	if (bSettingsGameOverrideEnabled)
	{
		const TSoftObjectPtr<UMoviePipelineGameOverrideSetting> GameOverrideSetting = Config->FindOrAddSettingByClass(UMoviePipelineGameOverrideSetting::StaticClass());
		if (GameOverrideSetting)
		{
			GameOverrideSetting->GameModeOverride = GameModeOverride;
			GameOverrideSetting->bCinematicQualitySettings = bCinematicQualitySettings;
			GameOverrideSetting->TextureStreaming = TextureStreaming;
			GameOverrideSetting->bUseLODZero = bUseLODZero;
			GameOverrideSetting->bDisableHLODs = bDisableHLODs;
			GameOverrideSetting->bUseHighQualityShadows = bUseHighQualityShadows;
			GameOverrideSetting->ShadowDistanceScale = ShadowDistanceScale;
			GameOverrideSetting->ShadowRadiusThreshold = ShadowRadiusThreshold;
			GameOverrideSetting->bOverrideViewDistanceScale = bOverrideViewDistanceScale;
			GameOverrideSetting->ViewDistanceScale = ViewDistanceScale;
			GameOverrideSetting->bFlushGrassStreaming = bFlushGrassStreaming;
		}
	}

	if (bSettingsUIEnabled)
	{
		const TSoftObjectPtr<UMoviePipelineWidgetRenderer> WidgetRenderer = Config->FindOrAddSettingByClass(UMoviePipelineWidgetRenderer::StaticClass());
		if (WidgetRenderer)
		{
			WidgetRenderer->bCompositeOntoFinalImage = bCompositeOntoFinalImage;
		}
	}

	if (bSettingsBurnInEnabled)
	{
		const TSoftObjectPtr<UMoviePipelineBurnInSetting> BurnInSetting = Config->FindOrAddSettingByClass(UMoviePipelineBurnInSetting::StaticClass());
		if (BurnInSetting)
		{
			BurnInSetting->BurnInClass = BurnInClass;
			BurnInSetting->bCompositeOntoFinalImage = bBurnInCompositeOntoFinalImage;
		}
	}

	if (bSettingsCameraEnabled)
	{
		const TSoftObjectPtr<UMoviePipelineCameraSetting> CameraSetting = Config->FindOrAddSettingByClass(UMoviePipelineCameraSetting::StaticClass());
		if (CameraSetting)
		{
			CameraSetting->OverscanPercentage = OverscanPercentage;
			CameraSetting->ShutterTiming = ShutterTiming;
		}
	}
	
	return Config;
}

bool UGamedevHelperRenderingSettings::IsValidJobSetting(TSoftObjectPtr<UMoviePipelineSetting> Setting)
{
	if (!Setting) return false;
	
	Setting->ValidateState();

	if (Setting->GetValidationState() == EMoviePipelineValidationState::Errors || Setting->GetValidationState() == EMoviePipelineValidationState::Warnings)
	{
		const TArray<FText> ValidationResults = Setting->GetValidationResults();

		ErrorMsg.Reset();
		for (const auto& ValidationResult : ValidationResults)
		{
			ErrorMsg.Append(ValidationResult.ToString() + TEXT("\n"));
		}

		return false;
	}

	return true;
}
