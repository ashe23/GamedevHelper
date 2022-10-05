// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "Settings/GdhRenderingSettings.h"
#include "GdhRenderingLibrary.generated.h"

class UGdhRenderingSettings;
class UGdhMovieRenderSettings;
class ULevelSequence;
class UMoviePipelineQueue;
class UMoviePipelineSetting;
class UMoviePipelineMasterConfig;

UCLASS()
class UGdhRenderingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Checks RenderingSettings parameters and returns error message (if any) as string
	 * @param RenderingSettings UGdhRenderingSettings
	 * @return FString
	 */
	static FString Check(const UGdhRenderingSettings* RenderingSettings);
	
	/**
	 * @brief Check MovieRenderingSettings parameters and return error message (if any) as string
	 * @param MovieRenderSettings UGdhMovieRenderSettings 
	 * @return FString
	 */
	static FString Check(const UGdhMovieRenderSettings* MovieRenderSettings);

	/**
	 * @brief Returns folder name based on RenderingSettings current resolution
	 * @param RenderingSettings UGdhRenderingSettings
	 * @return FString
	 */
	static FString GetResolutionFolderName(const UGdhRenderingSettings* RenderingSettings);

	/**
	 * @brief Returns image class based on RenderingSettings image format. By default return .png
	 * @param RenderingSettings UGdhRenderingSettings
	 * @return UClass
	 */
	static UClass* GetImageClass(const UGdhRenderingSettings* RenderingSettings);

	/**
	 * @brief Returns image extension based on RenderingSettings image format
	 * @param RenderingSettings UGdhRenderingSettings
	 * @param IncludeDot 
	 * @return FString
	 */
	static FString GetImageExtension(const UGdhRenderingSettings* RenderingSettings, const bool IncludeDot = false);
	
	/**
	 * @brief Returns video extension based on Rendering Settings video format
	 * @param RenderingSettings UGdhRenderingSettings
	 * @param IncludeDot 
	 * @return FString
	 */
	static FString GetVideoExtension(const UGdhRenderingSettings* RenderingSettings, const bool IncludeDot = false);
	
	static bool IsValidJobSetting(UMoviePipelineSetting* Setting);
	static bool IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig);
	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
	static uint32 GetRenderedFramesNum(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue, bool& bHasMissingFrames);
	static uint32 GetLevelSequenceDuration(const ULevelSequence* LevelSequence);
	static FString GetImageOutputDirectoryPath(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue);
	static FString GetVideoOutputDirectoryPath(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue);
	static FString GetFFmpegEncodeCmd(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue);
	static void RunFFmpegCommands(const TArray<FGdhFFmpegCommand>& FFmpegCommands);
};
