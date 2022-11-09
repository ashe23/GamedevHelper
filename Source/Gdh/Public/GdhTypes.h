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
enum class EGdhVertexAnimTextureType : uint8
{
	UV,
	Normal
};

UENUM(BlueprintType)
enum class EGdhNamingCase : uint8
{
	PascalCase UMETA(DisplayName = "PascalCase"),
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	KebabCase UMETA(DisplayName = "kebab-case"),
	CamelCase UMETA(DisplayName = "camelCase"),
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

USTRUCT(BlueprintType)
struct FGdhAssetFormat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asset Format")
	FString Prefix;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asset Format")
	FString Suffix;

	bool IsEmpty() const
	{
		return Prefix.IsEmpty() && Suffix.IsEmpty();
	}
};

struct FGdhFFmpegCommand
{
	FString SequenceName;
	FString Command;
};
