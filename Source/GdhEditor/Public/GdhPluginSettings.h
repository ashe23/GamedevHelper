// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GdhPluginSettings.generated.h"

/**
 * GamedevHelper Plugin main settings
 */
UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "GamedevHelper")
class UGdhPluginSettings : public UDeveloperSettings
{
public:

	GENERATED_BODY()

	UGdhPluginSettings();

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
	virtual FText GetSectionText() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// FFmpeg executable path. Plugin will try to detect it automatically from system env PATHS. But
	// you can modify it manually.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FFilePath FFmpegExePath;
};
