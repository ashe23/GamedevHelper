// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhRenderingAssetsSettings.generated.h"

DECLARE_DELEGATE(FGdhRenderingAssetsSettingsOnChangeDelegate);

class ULevelSequence;
class UMoviePipelineQueue;

UCLASS(Config = EditorPerProjectUserSettings, meta=(DisplayName="Rendering Assets Settings"))
class UGdhRenderingAssetsSettings final : public UObject
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="RenderingAssets", meta=(ToolTip="LevelSequences and Map assets mapping. If Map asset not specified, editor map will be used"))
	TMap<TSoftObjectPtr<ULevelSequence>, TSoftObjectPtr<UWorld>> LevelSequences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="RenderingAssets", meta=(ToolTip="MoviePipelineQueue assets, all jobs which will be rendered"))
	TSet<TSoftObjectPtr<UMoviePipelineQueue>> MoviePipelineQueues;

	FGdhRenderingAssetsSettingsOnChangeDelegate& OnChange();
private:
	FGdhRenderingAssetsSettingsOnChangeDelegate OnChangeDelegate;;
};
