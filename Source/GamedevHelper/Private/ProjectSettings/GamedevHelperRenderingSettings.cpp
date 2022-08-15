// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectSettings/GamedevHelperRenderingSettings.h"
#include "GamedevHelper.h"
#include "MoviePipelineImageSequenceOutput.h"

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
