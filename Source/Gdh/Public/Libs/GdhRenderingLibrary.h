// // Copyright Ashot Barkhudaryan. All Rights Reserved.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "GdhTypes.h"
// #include "Settings/GdhRenderingSettings.h"
// #include "GdhRenderingLibrary.generated.h"
//
// class UGdhRenderingSettings;
// class ULevelSequence;
// class UMoviePipelineQueue;
// class UMoviePipelineSetting;
// class UMoviePipelineMasterConfig;
//
// UCLASS()
// class UGdhRenderingLibrary : public UBlueprintFunctionLibrary
// {
// 	GENERATED_BODY()
// public:
// 	/**
// 	 * @brief Checks RenderingSettings parameters and returns error message (if any) as string
// 	 * @param RenderingSettings UGdhRenderingSettings
// 	 * @return FString
// 	 */
// 	static FString Check(const UGdhRenderingSettings* RenderingSettings);
// 	
// 	/**
// 	 * @brief Check MovieRenderingSettings parameters and return error message (if any) as string
// 	 * @param MovieRenderSettings UGdhMovieRenderSettings 
// 	 * @return FString
// 	 */
// 	// static FString Check(const UGdhMovieRenderSettings* MovieRenderSettings);
//
// 	/**
// 	 * @brief Returns folder name based on RenderingSettings current resolution
// 	 * @param RenderingSettings UGdhRenderingSettings
// 	 * @return FString
// 	 */
// 	static FString GetResolutionFolderName(const UGdhRenderingSettings* RenderingSettings);
//
// 	/**
// 	 * @brief Returns image class based on RenderingSettings image format. By default return .png
// 	 * @param ImageFormat EGdhImageFormat
// 	 * @return UClass
// 	 */
// 	static UClass* GetImageClass(const EGdhImageFormat ImageFormat);
//
// 	/**
// 	 * @brief Returns image extension based on RenderingSettings image format
// 	 * @param ImageFormat EGdhImageFormat
// 	 * @param IncludeDot 
// 	 * @return FString
// 	 */
// 	static FString GetImageExtension(const EGdhImageFormat ImageFormat, const bool IncludeDot = false);
// 	
// 	/**
// 	 * @brief Returns video extension based on Rendering Settings video format
// 	 * @param VideoFormat EGdhVideoFormat
// 	 * @param IncludeDot 
// 	 * @return FString
// 	 */
// 	static FString GetVideoExtension(const EGdhVideoFormat VideoFormat, const bool IncludeDot = false);
//
// 	/**
// 	 * @brief Returns resolution based on rendering settings presets
// 	 * @param RenderingSettings UGdhRenderingSettings
// 	 * @return FIntPoint
// 	 */
// 	static FIntPoint GetResolution(const UGdhRenderingSettings* RenderingSettings);
// 	
// 	static bool IsValidJobSetting(UMoviePipelineSetting* Setting);
// 	static bool IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig);
// 	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
// 	static uint32 GetRenderedFramesNum(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue, bool& bHasMissingFrames);
// 	static uint32 GetLevelSequenceDuration(const ULevelSequence* LevelSequence);
// 	static FString GetImageOutputDirectoryPath(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue);
// 	static FString GetVideoOutputDirectoryPath(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue);
// 	static FString GetFFmpegEncodeCmd(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue);
// 	static FString GetFFmpegEncodeCmdPreview(const UGdhRenderingSettings* RenderingSettings);
// 	static void RunFFmpegCommands(const TArray<FGdhFFmpegCommand>& FFmpegCommands);
// };
