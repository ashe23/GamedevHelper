// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelper.h"
#include "MoviePipelineQueue.h"
#include "GamedevHelperTypes.generated.h"

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return static_cast<EnumType>(Enum->GetIndexByName(FName(*String)));
}

UENUM(BlueprintType)
enum class EGamedevHelperNamingCase : uint8
{
	PascalCase UMETA(DisplayName = "PascalCase"),
	PascalSnakeCase UMETA(DisplayName = "Pascal_Snake_Case"),
	SnakeCase UMETA(DisplayName = "snake_case"),
	KebabCase UMETA(DisplayName = "kebab-case"),
	CamelCase UMETA(DisplayName = "camelCase"),
};

UENUM(BlueprintType)
enum class EGamedevHelperVertexAnimTexture : uint8
{
	UV,
	Normal,
};

UENUM(BlueprintType)
enum class EGamedevHelperBlueprintType : uint8
{
	None,
	Normal,
	Interface,
	MacroLibrary,
	FunctionLibrary
};

UENUM(BlueprintType)
enum class EGamedevHelperModalStatus : uint8
{
	None,
	Pending,
	Success,
	Fail,
};

UENUM(BlueprintType)
enum class EGamedevHelperAssetType : uint8
{
	Blueprint,
	EditorUtility,
	Material,
	Texture,
	FX,
	Animation,
	AI,
	Foliage,
	Sound,
	Slate,
	Paper2D,
	Misc,
	Custom
};

UENUM(BlueprintType)
enum class EGamedevHelperRenameStatus : uint8
{
	Ok,
	OkToRename,
	MissingSettings,
	DuplicateNameContentBrowser,
	DuplicateNamePreview
};

UENUM(BlueprintType)
enum class EGamedevHelperVideoFormat : uint8
{
	Mp4 UMETA(DisplayName = "mp4"),
	Mkv UMETA(DisplayName = "mkv"),
	Avi UMETA(DisplayName = "avi"),
};

UENUM(BlueprintType)
enum class EGamedevHelperImageFormat : uint8
{
	Png UMETA(DisplayName = "png"),
	Jpg UMETA(DisplayName = "jpg"),
	Bmp UMETA(DisplayName = "bmp"),
};

UENUM(BlueprintType)
enum class EGamedevHelperRendererStatus : uint8
{
	None UMETA(DisplayName = "None", Hidden),
	OK UMETA(DisplayName = "OK"),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error"),
};

UENUM(BlueprintType)
enum class EGamedevHelperResolutionPreset : uint8
{
	Res360P UMETA(DisplayName = "360p (480x360)", ToolTip = "360p"),
	Res480P UMETA(DisplayName = "480p (640x480)", ToolTip = "480p"),
	Res720P UMETA(DisplayName = "720p (1280x720)", ToolTip = "720p (HD)"),
	Res1080P UMETA(DisplayName = "1080p (1920x1080)", ToolTip = "1080p (Full HD)"),
	Res2160P UMETA(DisplayName = "2160p (3840x2160)", ToolTip = "2160p (4K)"),
	ResCustom UMETA(DisplayName = "Custom", ToolTip = "Custom resolution settings")
};

USTRUCT(BlueprintType)
struct FGamedevHelperAssetNameFormat
{
	GENERATED_BODY()

	FGamedevHelperAssetNameFormat() = default;

	bool IsEmpty() const
	{
		return Prefix.IsEmpty() && Suffix.IsEmpty();
	}

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetNameFormat")
	FString Prefix;

	UPROPERTY(EditAnywhere, Category = "GamedevHelper|AssetNameFormat", meta = (EditCondition = "!bAssetClassAsSuffix"))
	FString Suffix;
};

USTRUCT(BlueprintType)
struct FGamedevHelperAudioTrack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GamedevHelper|AudioTrack", meta = (ToolTip = "Audio track name"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GamedevHelper|AudioTrack", meta = (ToolTip = "Audio track path. Supported formats is .mp3 and .wav"))
	FFilePath Path;

	void Validate()
	{
		// if (Name.IsEmpty())
		// {
		// 	ErrorMsg = GamedevHelperStandardText::AudioTrackNameIsEmpty;
		// 	return;
		// }
		//
		// if (Path.FilePath.IsEmpty())
		// {
		// 	ErrorMsg = GamedevHelperStandardText::AudioTrackPathNotSpecified;
		// 	return;
		// }
		//
		// if (!FPaths::FileExists(Path.FilePath))
		// {
		// 	ErrorMsg = GamedevHelperStandardText::AudioTrackNotExist;
		// 	return;
		// }
		//
		// const FString Extension = FPaths::GetExtension(Path.FilePath, false).ToLower();
		// if (!Extension.Equals("mp3") && !Extension.Equals("wav"))
		// {
		// 	ErrorMsg = GamedevHelperStandardText::AudioTrackInvalidExtension;
		// 	return;
		// }

		ErrorMsg.Reset();
	}

	bool IsValid() const
	{
		return ErrorMsg.IsEmpty();
	}

	FString GetErrorMsg() const
	{
		return ErrorMsg;
	}

private:
	FString ErrorMsg;
};

USTRUCT()
struct FGamedevHelperFFmpegCommand
{
	GENERATED_BODY()

	FString QueueName;
	FString SequenceName;
	FString AudioTrack;
	FString Command;
	FString CommandTitle;

	bool IsValid() const
	{
		return !QueueName.IsEmpty() && !SequenceName.IsEmpty() && !Command.IsEmpty();
	}
};

USTRUCT(DisplayName="Render Item")
struct FGamedevHelperRenderElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="RenderItem", meta=(AllowedClasses="LevelSequence"))
	FSoftObjectPath LevelSequence;

	UPROPERTY(EditAnywhere, Category="RenderItem")
	bool bUseEditorMap = true;

	UPROPERTY(EditAnywhere, Category="RenderItem", meta=(AllowedClasses="World", EditCondition="!bUseEditorMap"))
	FSoftObjectPath Map;
};
