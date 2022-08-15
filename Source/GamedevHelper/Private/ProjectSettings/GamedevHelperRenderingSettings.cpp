// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectSettings/GamedevHelperRenderingSettings.h"
#include "GamedevHelper.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineImageSequenceOutput.h"
#include "MoviePipelineQueueSubsystem.h"

UGamedevHelperRenderingSettings::UGamedevHelperRenderingSettings()
{
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
	
	const TSoftObjectPtr<UMoviePipelineQueue> BaseQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (BaseQueue)
	{
		BaseQueue->DeleteAllJobs();
		const TSoftObjectPtr<UMoviePipelineExecutorJob> BaseJob = BaseQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
		const TSoftObjectPtr<UMoviePipelineMasterConfig> Config = BaseJob->GetConfiguration();
		Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
		Config->FindOrAddSettingByClass(GetImageClass());
		
		if (bSettingsAAEnabled)
		{
			const TSoftObjectPtr<UMoviePipelineAntiAliasingSetting> AntiAliasingSetting = Config->FindOrAddSettingByClass(UMoviePipelineAntiAliasingSetting::StaticClass());
			AntiAliasingSetting->SpatialSampleCount = SpatialSampleCount;
			AntiAliasingSetting->TemporalSampleCount = TemporalSampleCount;
			AntiAliasingSetting->bOverrideAntiAliasing = bOverrideAntiAliasing;
			AntiAliasingSetting->AntiAliasingMethod = AntiAliasingMethod;
			AntiAliasingSetting->RenderWarmUpCount = RenderWarmUpCount;
			AntiAliasingSetting->bUseCameraCutForWarmUp = bUseCameraCutForWarmUp;
			AntiAliasingSetting->EngineWarmUpCount = EngineWarmUpCount;
			AntiAliasingSetting->bRenderWarmUpFrames = bRenderWarmUpFrames;

			if (!IsValidJobSetting(AntiAliasingSetting))
			{
				return;
			}
		}
	}

	ErrorMsg.Reset();
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
