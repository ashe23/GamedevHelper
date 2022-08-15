// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperProjectSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings)
class UGamedevHelperAssetNamingConventionSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingConventionSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	FGamedevHelperAssetNameFormat GetNameFormatByAssetData(const FAssetData& Asset) const;
	FGamedevHelperAssetNameFormat GetNameFormatByClass(const UClass* SearchClass) const;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", DisplayName = "BlueprintPrefixes", meta = (ToolTip = "Blueprint prefixes based on blueprint type"))
	TMap<EGamedevHelperBlueprintType, FString> BlueprintTypePrefixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", DisplayName = "Asset Namings", meta = (ToolTip = "Asset class and name format mappings"))
	TMap<UClass*, FGamedevHelperAssetNameFormat> Mappings;
};

UCLASS(Config=EditorPerProjectUserSettings)
class UGamedevHelperWorldOutlinerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperWorldOutlinerSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	FName GetFolderNameByActor(const AActor* Actor);

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Move To Unsorted", meta = (ToolTip = "Move actors whos class is not specified, to 'Unsorted' folder"))
	bool bMoveToUnsorted = true;

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Unsorted folder name",
		meta = (ToolTip = "Unsorted folder name. Default 'Unsorted'", EditCondition = "bMoveToUnsorted"))
	FString UnsortedFolderName = TEXT("Unsorted");

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "World outliner mappings", meta = (ToolTip = "Actor class and folder name mappings"))
	TMap<UClass*, FName> Mappings;
};

// UCLASS(Config=EditorPerProjectUserSettings)
// class UGamedevHelperRenderingSettings : public UObject
// {
// 	GENERATED_BODY()
// public:
// 	UGamedevHelperRenderingSettings();
//
// 	bool IsValid() const;
// 	FString GetSubDirProject() const;
// 	FString GetSubDirImage() const;
// 	FString GetSubDirVideo() const;
// 	FString GetSubDirMixed() const;
// 	FString GetErrorMsg() const;
// 	FString GetResolutionAsString(const FString& Separator = TEXT(":")) const;
// 	FString GetVideoFormatAsString(const bool IncludeDot = false) const;
// 	FString GetImageFormatAsString(const bool IncludeDot = false) const;
// 	FIntPoint GetResolution() const;
// 	UClass* GetMoviePipelineOutputSettingImageClass() const;
// 	FString GetFileNameFormat() const;
// 	FString GetJsonFilePath() const;
//
// #if WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
// #endif
//
// 	UPROPERTY(EditAnywhere, Config, Category="Rendering Settings|General", meta = (ToolTip = "Path to ffmpeg.exe file", FilePathFilter = "exe"))
// 	FFilePath FFmpegExe;
//
// 	UPROPERTY(EditAnywhere, Config, Category="Rendering Settings|General", meta = (ToolTip = "Export directory for rendered images or videos.By default will be under 'Saved/{ProjectName}/' folder"))
// 	FDirectoryPath OutputDirectory;
// 	
// 	UPROPERTY(EditAnywhere, Config, Category = "Rendering Settings|Format", DisplayName = "Resolution", meta = (ToolTip = "Resolution presets. By Default is 1080p"))
// 	EGamedevHelperRendererResolutionPreset ResolutionPreset;
//
// 	UPROPERTY(EditAnywhere, Config, Category = "Rendering Settings|Format", DisplayName = "Custom Resolution",
// 		meta = (
// 			ToolTip = "Max supported resolution: 4096",
// 			EditCondition = "ResolutionPreset == EGamedevHelperRendererResolutionPreset::ResCustom",
// 			UIMin="1", UIMax="4096",
// 			ClampMin="1", ClampMax="4096"))
// 	FIntPoint CustomResolution;
//
// 	UPROPERTY(EditAnywhere, Config, Category = "Rendering Settings|Format", DisplayName = "Video Format", meta = (ToolTip = "Output video format. Default is .mp4"))
// 	EGamedevHelperRendererVideoFormat VideoFormat;
//
// 	UPROPERTY(EditAnywhere, Config, Category = "Rendering Settings|Format", DisplayName = "Image Format", meta = (ToolTip = "Output image format. Default is .png"))
// 	EGamedevHelperRendererImageFormat ImageFormat;
//
// 	UPROPERTY(EditAnywhere, Config, Category = "Rendering Settings|Format", DisplayName = "Framerate", meta = (ToolTip = "Output framerate. Default is 30 fps"))
// 	FFrameRate Framerate;
//
// 	UPROPERTY(EditAnywhere, Category = "Rendering Settings|Format", DisplayName = "AntiAlias", meta = (ToolTip = "Output framerate. Default is 30 fps"))
// 	class UMoviePipelineAntiAliasingSetting* AntiAliasingSetting = nullptr;
//
// protected:
// 	FString ErrorMsg;
// 	FIntPoint CurrentResolution;
//
// private:
// 	void CheckSubFoldersIntegrity() const;
// 	void Validate();
// };
