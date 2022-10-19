// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhRenderingSettings.h"
#include "Gdh.h"

UGdhRenderingSettings::UGdhRenderingSettings()
{
	FFmpegFlags.Add(TEXT("-c:v libx264"));
	FFmpegFlags.Add(TEXT("-preset slow"));
	FFmpegFlags.Add(TEXT("-tune film"));
	FFmpegFlags.Add(TEXT("-crf 18"));
	FFmpegFlags.Add(TEXT("-pix_fmt yuv420p"));

	// FFmpegEncodeCmdPreview = UGdhRenderingLibrary::GetFFmpegEncodeCmdPreview(this);
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
	
	// FFmpegEncodeCmdPreview = UGdhRenderingLibrary::GetFFmpegEncodeCmdPreview(this);
	
	SaveConfig();

	if (OnChangeDelegate.IsBound())
	{
		OnChangeDelegate.Execute();
	}
}
#endif

FGdhRenderingSettingsOnChangeDelegate& UGdhRenderingSettings::OnChange()
{
	return OnChangeDelegate;
}
