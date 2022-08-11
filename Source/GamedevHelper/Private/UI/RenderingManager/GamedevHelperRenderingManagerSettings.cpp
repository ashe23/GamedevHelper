// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerSettings.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineImageSequenceOutput.h"

UGamedevHelperRenderingManagerSettings::UGamedevHelperRenderingManagerSettings()
{
	ResolutionPreset = EGamedevHelperRendererResolutionPreset::Res1080P;
	Resolution = GamedevHelperConstants::Resolution1080P;
	CustomResolution = GamedevHelperConstants::Resolution1080P;
	VideoFormat = EGamedevHelperRendererVideoFormat::Mp4;
	ImageFormat = EGamedevHelperRendererImageFormat::Png;
	Framerate = GamedevHelperConstants::DefaultFrameRate;
}

FIntPoint UGamedevHelperRenderingManagerSettings::GetResolution() const
{
	return Resolution;
}

FString UGamedevHelperRenderingManagerSettings::GetResolutionAsString(const FString& Separator) const
{
	if (Separator.IsEmpty()) return Separator;

	return FString::Printf(TEXT("%d%s%d"), Resolution.X, *Separator, Resolution.Y);
}

FString UGamedevHelperRenderingManagerSettings::GetVideoFormatAsString(const bool IncludeDot) const
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

FString UGamedevHelperRenderingManagerSettings::GetImageFormatAsString(const bool IncludeDot) const
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

UClass* UGamedevHelperRenderingManagerSettings::GetMoviePipelineOutputSettingImageClass() const
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

FString UGamedevHelperRenderingManagerSettings::GetErrorText() const
{
	return ErrorText;
}

#if WITH_EDITOR
void UGamedevHelperRenderingManagerSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty->GetFName().IsEqual("OutputDir"))
	{
		if (!OutputDir.Path.IsEmpty() && FPaths::DirectoryExists(FPaths::ConvertRelativePathToFull(OutputDir.Path)))
		{
			CreateOutputSubdirectories();
		}
	}
	
	SaveConfig();
}
#endif

bool UGamedevHelperRenderingManagerSettings::IsValid()
{
	if (FFmpegExePath.FilePath.IsEmpty())
	{
		ErrorText = GamedevHelperStandardText::ErrFFmpegExePathNotSpecified;
		return false;
	}

	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(FFmpegExePath.FilePath)))
	{
		ErrorText = GamedevHelperStandardText::ErrFFmpegExePathDoestNotExist;
		return false;
	}

	if (OutputDir.Path.IsEmpty())
	{
		ErrorText = GamedevHelperStandardText::ErrOutputDirNotSpecified;
		return false;
	}

	if (!FPaths::DirectoryExists(FPaths::ConvertRelativePathToFull(OutputDir.Path)))
	{
		ErrorText = GamedevHelperStandardText::ErrOutputDirDoesNotExist;
		return false;
	}

	ErrorText = TEXT("");
	
	return true;
}

void UGamedevHelperRenderingManagerSettings::CreateOutputSubdirectories() const
{
	if (OutputDir.Path.IsEmpty()) return;
	
	
	const FString SubDirectoriesRoot = FPaths::ConvertRelativePathToFull(FString::Printf(TEXT("%s/%s/"), *OutputDir.Path, FApp::GetProjectName()));

	if (!FPaths::DirectoryExists(SubDirectoriesRoot))
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (!PlatformFile.CreateDirectoryTree(*SubDirectoriesRoot))
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Failed to create directory %s"), *SubDirectoriesRoot);
		}

		// our OutputDirectory folder must always contain subdirectories
		// - image - Contains all rendered images
		// - video - Contains only video files without audio
		// - mixed - Contains video files mixed with audio

		TArray<FString> SubDirectories;
		SubDirectories.Reserve(3);
		SubDirectories.Add(TEXT("image"));
		SubDirectories.Add(TEXT("video"));
		SubDirectories.Add(TEXT("mixed"));

		for (const auto& Dir : SubDirectories)
		{
			const FString Path = FString::Printf(TEXT("%s/%s"), *SubDirectoriesRoot, *Dir);
			if (!PlatformFile.CreateDirectoryTree(*Path))
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Failed to create directory %s"), *Path);
			}
		}
	}
}
