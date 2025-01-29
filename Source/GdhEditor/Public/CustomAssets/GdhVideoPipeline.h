// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoPipeline.generated.h"

// Video Pipeline assets are used in order to crete specific render and encoding settings, when using Video Encoder Tool
// So you can have multiple pipeline assets with different settings, for example one pipeline for mp4 encoding with
// FullHD resolution another , for mkv or hls with 360p resolution etc. So you specify settings one time in this asset,
// then you can render same level sequence using movie render with specified settings
UCLASS(BlueprintType)
class GDHEDITOR_API UGdhVideoPipeline : public UObject
{
  public:

	GENERATED_BODY()

	UPROPERTY(
		EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (ToolTip = "Main output directory for rendered images and encoded videos")
	)
	FDirectoryPath OutputDir;

	UPROPERTY(
		BlueprintReadWrite, EditAnywhere, Category = "General",
		meta = (ToolTip = "Directory name for rendered images. Will be under OutputDir folder")
	)
	FString DirImages = TEXT("images");

	UPROPERTY(
		BlueprintReadWrite, EditAnywhere, Category = "General",
		meta = (ToolTip = "Directory name for encoded videos. Will be under OutputDir folder")
	)
	FString DirVideo = TEXT("video");

	UPROPERTY(
		BlueprintReadWrite, EditAnywhere, Category = "Encode",
		meta = (ToolTip = "Encoding command to apply on every rendered image sequence")
	)
	TArray<FString> EncodeCmd;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Encode", meta = (ToolTip = "Encode cmd visualizer"))
	FString EncodeCmdVis;

	TArray<TSoftObjectPtr<UMoviePipelineQueue>>

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Gdh")
	void EncodeCmdUpdate();
};
