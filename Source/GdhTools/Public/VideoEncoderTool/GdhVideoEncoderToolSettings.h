// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoEncoderToolSettings.generated.h"

class UGdhVideoPipeline;

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhVideoEncoderToolSettings : public UObject
{
  public:

	GENERATED_BODY()

	UGdhVideoEncoderToolSettings();

	// Path to ffmpeg executable, if left empty will use path from ENV, if exists
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	FString PathFfmpegExe = TEXT("ffmpeg.exe");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	TSoftObjectPtr<UGdhVideoPipeline> Pipeline;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
