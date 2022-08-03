// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/Renderer/GamedevHelperRendererSettings.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineImageSequenceOutput.h"

UGamedevHelperRendererSettings::UGamedevHelperRendererSettings()
{
	bUseMovieRenderPresetSettings = false;
	Resolution = GamedevHelperConstants::Resolution1080P;
	CustomResolution = GamedevHelperConstants::Resolution1080P;
	ResolutionPreset = EGamedevHelperRendererResolutionPreset::Res1080P;
	VideoFormat = EGamedevHelperRendererVideoFormat::Mp4;
	ImageFormat = EGamedevHelperRendererImageFormat::Png;
	Framerate = GamedevHelperConstants::DefaultFrameRate;
	PlaybackSpeed = 1.0f;
	bRenderUI = false;
	AdditionalCmdFlags = TEXT("");
	bCanUseCustomResolution = false;
}

FIntPoint UGamedevHelperRendererSettings::GetResolution() const
{
	return Resolution;
}

FString UGamedevHelperRendererSettings::GetResolutionAsString(const FString& Separator) const
{
	if (Separator.IsEmpty()) return Separator;

	return FString::Printf(TEXT("%d%s%d"), Resolution.X, *Separator, Resolution.Y);
}

FString UGamedevHelperRendererSettings::GetVideoFormatAsString(const bool IncludeDot) const
{
	switch (VideoFormat)
	{
		case EGamedevHelperRendererVideoFormat::Mp4:
			return IncludeDot ? TEXT(".mp4") : TEXT("mp4");
		case EGamedevHelperRendererVideoFormat::Mkv:
			return IncludeDot ? TEXT(".mkv") : TEXT("mkv");
		case EGamedevHelperRendererVideoFormat::Avi:
			return IncludeDot ? TEXT(".avi") : TEXT("avi");
		default:
			return TEXT("None");
	}
}

FString UGamedevHelperRendererSettings::GetImageFormatAsString(const bool IncludeDot) const
{
	switch (ImageFormat)
	{
		case EGamedevHelperRendererImageFormat::Png:
			return IncludeDot ? TEXT(".png") : TEXT("png");
		case EGamedevHelperRendererImageFormat::Jpg:
			return IncludeDot ? TEXT(".jpeg") : TEXT("jpeg");
		case EGamedevHelperRendererImageFormat::Bmp:
			return IncludeDot ? TEXT(".bmp") : TEXT("bmp");
		default:
			return TEXT("None");
	}
}

UClass* UGamedevHelperRendererSettings::GetMoviePipelineOutputSettingImageClass() const
{
	switch (ImageFormat)
	{
		case EGamedevHelperRendererImageFormat::Png:
			return UMoviePipelineImageSequenceOutput_PNG::StaticClass();
		case EGamedevHelperRendererImageFormat::Jpg:
			return UMoviePipelineImageSequenceOutput_JPG::StaticClass();
		case EGamedevHelperRendererImageFormat::Bmp:
			return UMoviePipelineImageSequenceOutput_BMP::StaticClass();
		default:
			return nullptr;
	}
}


#if WITH_EDITOR
void UGamedevHelperRendererSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	bCanUseCustomResolution = bUseMovieRenderPresetSettings == false && ResolutionPreset == EGamedevHelperRendererResolutionPreset::ResCustom;

	SaveConfig();
}
#endif

bool UGamedevHelperRendererSettings::IsValid() const
{
	if (FFmpegExePath.FilePath.IsEmpty())
	{
		return false;
	}

	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(FFmpegExePath.FilePath)))
	{
		return false;
	}

	if (OutputDir.Path.IsEmpty())
	{
		return false;
	}

	if (!FPaths::DirectoryExists(FPaths::ConvertRelativePathToFull(OutputDir.Path)))
	{
		return false;
	}

	// todo:ashe23 add all other parameters validation

	return true;
}
