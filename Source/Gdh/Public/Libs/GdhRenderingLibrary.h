// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhRenderingLibrary.generated.h"

class ULevelSequence;
class UMoviePipelineQueue;
class UMoviePipelineSetting;
class UMoviePipelineMasterConfig;

UCLASS()
class UGdhRenderingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
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
