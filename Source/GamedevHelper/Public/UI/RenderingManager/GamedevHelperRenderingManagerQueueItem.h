// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerQueueItem.generated.h"

class UMoviePipelineSetting;

UCLASS(Transient)
class UGamedevHelperRenderingManagerQueueItem : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperRenderingManagerQueueItem();
	
	UPROPERTY()
	EGamedevHelperRendererStatus Status;

	UPROPERTY()
	FString QueueName;
	
	UPROPERTY()
	FString SequenceName;
	
	UPROPERTY()
	FString SequenceDuration;
	
	UPROPERTY()
	int32 SequenceDurationInFrames = 0;
	
	UPROPERTY()
	int32 SequenceStartFrame = 0;
	
	UPROPERTY()
	int32 SequenceRenderedFrames = 0;
	
	UPROPERTY()
	FString SequenceOutputDir;
	
	UPROPERTY()
	FString Note;
	
private:
	UPROPERTY()
	FGamedevHelperRenderingManagerQueueItemData Data;
	
	// UPROPERTY()
	// FSoftObjectPath LevelSequence;
	//
	// UPROPERTY()
	// FSoftObjectPath Map;
	//
	// UPROPERTY()
	// TArray<UMoviePipelineSetting*> UserSettings;
public:
	bool IsRendered() const;
	bool IsValid() const;
	int32 GetRenderedFramesNum() const;
	void Validate(const FGamedevHelperRenderingManagerQueueItemData& QueueItemData);
};