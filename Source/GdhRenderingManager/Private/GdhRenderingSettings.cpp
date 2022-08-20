// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingSettings.h"
#include "GdhRenderingManager.h"
// Engine Headers
#include "MoviePipelineImageSequenceOutput.h"

UGdhRenderingSettings::UGdhRenderingSettings()
{
}

FName UGdhRenderingSettings::GetContainerName() const
{
	return FName{"Project"};
}

FName UGdhRenderingSettings::GetCategoryName() const
{
	return FName{"GamedevHelper"};
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
			CurrentResolution = GdhRenderingManagerConstants::Resolution360P;
			break;
		case EGdhResolutionPreset::Res480P:
			CurrentResolution = GdhRenderingManagerConstants::Resolution480P;
			break;
		case EGdhResolutionPreset::Res720P:
			CurrentResolution = GdhRenderingManagerConstants::Resolution720P;
			break;
		case EGdhResolutionPreset::Res1080P:
			CurrentResolution = GdhRenderingManagerConstants::Resolution1080P;
			break;
		case EGdhResolutionPreset::Res1440P:
			CurrentResolution = GdhRenderingManagerConstants::Resolution1440P;
			break;
		case EGdhResolutionPreset::Res2160P:
			CurrentResolution = GdhRenderingManagerConstants::Resolution2160P;
			break;
		case EGdhResolutionPreset::ResCustom:
			CurrentResolution = CustomResolution;
			break;
		default:
			CurrentResolution = GdhRenderingManagerConstants::Resolution1080P;
	}

	SaveConfig();
}
#endif

void UGdhRenderingSettings::Validate()
{
	if (!ValidateOutputDirectory())
	{
		return;
	}

	if (!ValidateFFmpegExe())
	{
		return;
	}

	if (!ValidateResolution())
	{
		return;
	}

	bIsValid = true;
	Super::Validate();
}

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

bool UGdhRenderingSettings::ValidateOutputDirectory()
{
	if (OutputDirectory.Path.IsEmpty())
	{
		SetErrorMsg(TEXT("Output directory not specified"));
		bIsValid = false;
		return false;
	}

	if (!FPaths::DirectoryExists(OutputDirectory.Path))
	{
		SetErrorMsg(TEXT("Output directory does not exist"));
		bIsValid = false;
		return false;
	}

	return true;
}

bool UGdhRenderingSettings::ValidateFFmpegExe()
{
	if (FFmpegExe.FilePath.ToLower().Equals(TEXT("ffmpeg.exe")))
	{
		return true;
	}

	if (FFmpegExe.FilePath.IsEmpty())
	{
		SetErrorMsg(TEXT("FFmpeg exe not specified"));
		bIsValid = false;
		return false;
	}

	if (!FPaths::FileExists(FFmpegExe.FilePath))
	{
		SetErrorMsg(TEXT("FFmpeg exe not found"));
		bIsValid = false;
		return false;
	}

	return true;
}

bool UGdhRenderingSettings::ValidateResolution()
{
	if (CurrentResolution.X % 2 != 0 || CurrentResolution.Y % 2 != 0)
	{
		SetErrorMsg(TEXT("Resolution must have dimensions divisible by 2"));
		bIsValid = false;
		return false;
	}

	return false;
}
