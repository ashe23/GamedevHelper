// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGdh, Log, All);

namespace GdhConstants
{
	// General
	static const FName ModuleFullName{TEXT("GamedevHelper")};
	static const FName ModuleName{TEXT("Gdh")}; // Gdh - GamedevHelper
	static const FName ModuleStylesName{TEXT("GamedevHelperStyle")};
	static const FName TabRenderingManager{TEXT("TabRenderingManager")};
	static const FString FFmpegJsonCommandsFile{TEXT("ffmpeg_commands.json")};
	static const FString FFmpegPythonFile{TEXT("ffmpeg_cli_runner.py")};

	// Rendering Settings
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
	static const FString AlphaLower{TEXT("abcdefghijklmnopqrstuvwxyz")};
	static const FString AlphaUpper{AlphaLower.ToUpper()};
	static const FString AlphaMixed{AlphaLower + AlphaLower.ToUpper()};
	static const FString Digits{TEXT("0123456789")};
}

class FGdh final : public IModuleInterface
{
public:
	// overrides
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	virtual bool IsGameModule() const override;

	void RegisterStyles();
	void RegisterCommands();
	void RegisterTabs();
	void RegisterMainMenu();
	void RegisterContextMenuActions();
	void UnregisterStyles();
	void UnregisterCommands();
	void UnregisterTabs();
private:
	TSharedPtr<FUICommandList> Commands;
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
};
