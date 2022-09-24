// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhRenderingSettings.h"
#include "Gdh.h"
// Engine Headers
#include "MoviePipelineImageSequenceOutput.h"
#include "Kismet/KismetStringLibrary.h"


UGdhRenderingSettings::UGdhRenderingSettings()
{
	FFmpegFlags.Add(TEXT("-c:v libx264"));
	FFmpegFlags.Add(TEXT("-profile:v high"));
	FFmpegFlags.Add(TEXT("-preset slow"));
	FFmpegFlags.Add(TEXT("-tune film"));
	FFmpegFlags.Add(TEXT("-crf 18"));
	FFmpegFlags.Add(TEXT("-pix_fmt yuv420p"));

	FFmpegEncodeCmdPreview = GetEncodeCmdPreview();
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

	FFmpegEncodeCmdPreview = GetEncodeCmdPreview();

	SaveConfig();

	if (GdhRenderingSettingsOnChangeDelegate.IsBound())
	{
		GdhRenderingSettingsOnChangeDelegate.Execute();
	}
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

FString UGdhRenderingSettings::GetEncodeCmdPreview() const
{
	return FString::Printf(
		TEXT("{ffmpeg_exe_path} -y -framerate %.1f -i {input_image_path}.%s -vf scale=%d:%d %s {output_video_path}.%s"),
		Framerate.AsDecimal(),
		*GetImageExtension(),
		CurrentResolution.X,
		CurrentResolution.Y,
		*UKismetStringLibrary::JoinStringArray(FFmpegFlags, TEXT(" ")),
		*GetVideoExtension()
	);
}
