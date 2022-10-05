// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhRenderingSettings.h"
#include "Gdh.h"
// Engine Headers
#include "MoviePipelineImageSequenceOutput.h"

UGdhRenderingSettings::UGdhRenderingSettings()
{
	FFmpegFlags.Add(TEXT("-c:v libx264"));
	FFmpegFlags.Add(TEXT("-profile:v high"));
	FFmpegFlags.Add(TEXT("-preset slow"));
	FFmpegFlags.Add(TEXT("-tune film"));
	FFmpegFlags.Add(TEXT("-crf 18"));
	FFmpegFlags.Add(TEXT("-pix_fmt yuv420p"));

	// FFmpegEncodeCmdPreview = GetEncodeCmdPreview();
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

	// FFmpegEncodeCmdPreview = GetEncodeCmdPreview();

	SaveConfig();

	if (OnChangeDelegate.IsBound())
	{
		OnChangeDelegate.Execute();
	}
}
#endif

FIntPoint UGdhRenderingSettings::GetResolution() const
{
	return CurrentResolution;
}

FGdhRenderingSettingsOnChangeDelegate& UGdhRenderingSettings::OnChange()
{
	return OnChangeDelegate;
}

// FString UGdhRenderingSettings::GetEncodeCmdPreview() const
// {
// 	return FString::Printf(
// 		TEXT("{ffmpeg_exe_path} -y -framerate %.3f -i {input_image_path}.%s -vf scale=%d:%d %s {output_video_path}.%s"),
// 		Framerate.AsDecimal(),
// 		*GetImageExtension(),
// 		CurrentResolution.X,
// 		CurrentResolution.Y,
// 		*UKismetStringLibrary::JoinStringArray(FFmpegFlags, TEXT(" ")),
// 		*GetVideoExtension()
// 	);
// }
