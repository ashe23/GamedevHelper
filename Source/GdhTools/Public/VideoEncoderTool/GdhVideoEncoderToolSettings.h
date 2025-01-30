// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoEncoderToolSettings.generated.h"

class UGdhRenderList;
class UMoviePipelineQueue;
class UMoviePipelineMasterConfig;

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhVideoEncoderToolSettings : public UObject
{
public:

	GENERATED_BODY()

	UGdhVideoEncoderToolSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	TSoftObjectPtr<UMoviePipelineMasterConfig> RenderSettings;

	// TODO:ashe23 this should be array later
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	TSoftObjectPtr<UMoviePipelineQueue> RenderQueue;

	// TODO:ashe23 this should be in separate custom asset file
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	TArray<FString> EncodeCmd;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	// TSoftObjectPtr<UGdhRenderList> RenderList;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
