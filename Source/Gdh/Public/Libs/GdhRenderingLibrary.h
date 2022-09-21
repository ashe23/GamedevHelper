// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhRenderingLibrary.generated.h"

class ULevelSequence;
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
	static uint32 GetRenderedFramesNum(const ULevelSequence* LevelSequence, bool& bHasMissingFrames);
	static uint32 GetLevelSequenceDuration(const ULevelSequence* LevelSequence);
	static FString GetImageOutputDirectoryPath(const ULevelSequence* LevelSequence);
	static FString GetVideoOutputDirectoryPath(const ULevelSequence* LevelSequence);
	static FString GetFFmpegEncodeCmd(const ULevelSequence* LevelSequence);
	static void RunFFmpegCommands(const TArray<FGdhFFmpegCommand>& FFmpegCommands);
};
