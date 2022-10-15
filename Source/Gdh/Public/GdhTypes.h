// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "GdhTypes.generated.h"

UENUM(BlueprintType)
enum class EGdhGenericStatus : uint8
{
	None UMETA(DisplayName = "None", Hidden),
	OK UMETA(DisplayName = "OK"),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error"),
};

UENUM(BlueprintType)
enum class EGdhModalStatus : uint8
{
	None UMETA(DisplayName = "None"),
	OK UMETA(DisplayName = "OK"),
	Pending UMETA(DisplayName = "Pending"),
	Error UMETA(DisplayName = "Error"),
};

UENUM(BlueprintType)
enum class EGdhImageFormat : uint8
{
	Png UMETA(DisplayName = "png"),
	Jpg UMETA(DisplayName = "jpg"),
	Bmp UMETA(DisplayName = "bmp"),
};

UENUM(BlueprintType)
enum class EGdhVideoFormat : uint8
{
	Mp4 UMETA(DisplayName = "mp4"),
	Mkv UMETA(DisplayName = "mkv"),
	Avi UMETA(DisplayName = "avi"),
};

UENUM(BlueprintType)
enum class EGdhResolutionPreset : uint8
{
	Res360P UMETA(DisplayName = "360p (480x360)", ToolTip = "360p"),
	Res480P UMETA(DisplayName = "480p (640x480)", ToolTip = "480p"),
	Res720P UMETA(DisplayName = "720p (1280x720)", ToolTip = "720p (HD)"),
	Res1080P UMETA(DisplayName = "1080p (1920x1080)", ToolTip = "1080p (Full HD)"),
	Res1440P UMETA(DisplayName = "1440p (2560x1440)", ToolTip = "1440p (QHD)"),
	Res2160P UMETA(DisplayName = "2160p (3840x2160)", ToolTip = "2160p (4K)"),
	ResCustom UMETA(DisplayName = "Custom", ToolTip = "Custom resolution settings")
};

UENUM(BlueprintType)
enum class EGdhRenderingMethod : uint8
{
	RenderImages UMETA(DisplayName = "RenderImagesOnly", ToolTip = "Renders images only using MovieRender, nothing will be encoded"),
	EncodeVideos UMETA(DisplayName = "EncodeVideosOnly", ToolTip = "Encode video files from rendered images only"),
	RenderAndEncode UMETA(DisplayName = "RenderAndEncode", ToolTip = "Will render images and then encode video files from them")
};

USTRUCT(BlueprintType)
struct FGdhLevelSequenceRenderSettings
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	TSoftObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	bool bUseEditorMap = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta=(EditCondition="bUseEditorMap == false"))
	TSoftObjectPtr<UWorld> Map;
};

struct FGdhFFmpegCommand
{
	FString SequenceName;
	FString Command;
};