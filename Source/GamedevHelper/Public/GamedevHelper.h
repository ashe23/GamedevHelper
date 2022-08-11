// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

namespace GamedevHelperConstants
{
	static const FName ModuleName = TEXT("GamedevHelper");
	static const FName TabAssetNamingManager{TEXT("TabAssetNamingManager")};
	static const FName TabWorldOutlinerManager{TEXT("TabWorldOutlinerManager")};
	static const FName TabRenderingManager{TEXT("TabRendererWindow")};
	static const FString AlphaLower = TEXT("abcdefghijklmnopqrstuvwxyz");
	static const FString AlphaUpper = AlphaLower.ToUpper();
	static const FString AlphaMixed = AlphaLower + AlphaLower.ToUpper();
	static const FString Digits = TEXT("0123456789");
	static constexpr int32 MaxResolution{4096};
	static const FIntPoint Resolution360P{480, 360};
	static const FIntPoint Resolution480P{640, 480};
	static const FIntPoint Resolution720P{1280, 720};
	static const FIntPoint Resolution1080P{1920, 1080};
	static const FIntPoint Resolution2160P{3840, 2160};
	static const FFrameRate DefaultFrameRate{30, 1};
}

namespace GamedevHelperStandardText
{
	static const FString ConfiguringStaticMeshes{TEXT("Configuring StaticMeshes for vertex animation")};
	static const FString ConfiguringStaticMeshesSuccess{TEXT("StaticMeshes configured successfully for vertex animation")};
	static const FString ConfiguringStaticMeshesFail{TEXT("Failed to configure some StaticMeshes")};
	static const FString ConfiguringTextures{TEXT("Configuring textures for vertex animation")};
	static const FString ConfiguringTexturesSuccess{TEXT("Textures configured successfully")};
	static const FString ConfiguringTexturesFail{TEXT("Failed to configure some textures")};
	static const FString DisablingStaticMeshesCollision{TEXT("Disabling collisions on StaticMeshes")};
	static const FString DisablingStaticMeshesCollisionSuccess{TEXT("Collisions disabled successfully")};
	static const FString DisablingStaticMeshesCollisionFail{TEXT("Failed to disable collisions")};
	static const FString ErrFFmpegExePathNotSpecified{TEXT("Error: FFmpeg exe path not specified")};
	static const FString ErrFFmpegExePathDoestNotExist{TEXT("Error: FFmpeg exe path does not exist")};
	static const FString ErrOutputDirNotSpecified{TEXT("Error: Output directory not specified")};
	static const FString ErrOutputDirDoesNotExist{TEXT("Error: Output directory does not exist")};
	static const FString ErrFailedToLoadAsset{TEXT("Error: Failed to load asset")};
	static const FString ErrNoJobsInQueue{TEXT("Error: No jobs in queue")};
	static const FString ErrInvalidQueue{TEXT("Error: Invalid queue asset")};
	static const FString PreparingQueueJobs{TEXT("Error: Invalid queue asset")};
	static const FString PreparingQueue{TEXT("Preparing render queue...")};
	static const FString JobInvalid{TEXT("Invalid Job Settings")};
	static const FString JobMissingLevelSequence{TEXT("Missing LevelSequence")};
	static const FString JobMissingMap{TEXT("Missing Map")};
	static const FString JobInvalidConfigs{TEXT("Invalid configs")};
	static const FString JobCantLoadLevelSequence{TEXT("Could not load LevelSequence asset")};
	static const FString JobNameNotSpecified{TEXT("Job name not specified")};
	static const FString QueueIsEmpty{TEXT("QueueAssets is empty")};
}

class IGamedevHelper : public IModuleInterface
{
public:
	static IGamedevHelper& Get()
	{
		return FModuleManager::LoadModuleChecked<IGamedevHelper>(GamedevHelperConstants::ModuleName);
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(GamedevHelperConstants::ModuleName);
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogGamedevHelper, Log, All);
