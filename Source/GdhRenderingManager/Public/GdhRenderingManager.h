// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhRenderingManagerTypes.h"
#include "Modules/ModuleInterface.h"

namespace GdhRenderingManagerConstants
{
	static const FName ModuleName{TEXT("GdhRenderingManager")};
	static const FString FFmpegLocationPyScript{TEXT("ffmpeg_location.py")};
	static const FIntPoint DefaultResolution{1920, 1080};
	static const FFrameRate DefaultFrameRate{30, 1};
	static constexpr EGdhImageFormat DefaultImageFormat{EGdhImageFormat::Png};
	static constexpr EGdhResolutionPreset DefaultResolutionPreset{EGdhResolutionPreset::Res1080P};
	static const FIntPoint Resolution360P{480, 360};
	static const FIntPoint Resolution480P{640, 480};
	static const FIntPoint Resolution720P{1280, 720};
	static const FIntPoint Resolution1080P{1920, 1080};
	static const FIntPoint Resolution1440P{2560, 1440};
	static const FIntPoint Resolution2160P{3840, 2160};
}

// This module responsible for rendering images from LevelSequences or MoviePipelineQueue assets, and encoding them into video files
class FGdhRenderingManager : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;
};