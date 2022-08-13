// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperRenderingManagerQueueItemUI.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerQueueSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings)
class UGamedevHelperRenderingManagerQueueSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperRenderingManagerQueueSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	void Validate();
	bool IsValid() const;
	FString GetErrorMsg() const;
	const TObjectPtr<UMoviePipelineQueue>& GetCustomPipeline() const;
	const TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>& GetQueueItems() const;
	const TArray<FGamedevHelperFFmpegCommand>& GetFFmpegCommands() const;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category="RenderingQueue", meta = (ToolTip = "MoviePipelineQueue assets to render", AllowedClasses = "MoviePipelineQueue"))
	TArray<FSoftObjectPath> QueueAssets;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category="RenderingQueue", meta = (ToolTip = "Audio Tracks that must be used when mixing with all sequences in queue", AllowedClasses = "MoviePipelineQueue"))
	TMap<FSoftObjectPath, FGamedevHelperAudioTrack> AudioTracks;

	
protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>> QueueItems;
	
	UPROPERTY()
	TObjectPtr<UMoviePipelineQueue> CustomPipeline;

	UPROPERTY()
	TArray<FGamedevHelperFFmpegCommand> FFmpegCommands;

private:
	bool bIsValid = false;
	FString ErrorMsg;
};