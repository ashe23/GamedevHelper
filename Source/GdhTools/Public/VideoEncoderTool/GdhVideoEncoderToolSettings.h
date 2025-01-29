// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoEncoderToolSettings.generated.h"

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhVideoEncoderToolSettings : public UObject
{
  public:

	GENERATED_BODY()

	UGdhVideoEncoderToolSettings();

	// Path to ffmpeg executable, if left empty will use path from ENV, if exists
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gdh")
	FString PathFfmpegExe = TEXT("ffmpeg.exe");

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
