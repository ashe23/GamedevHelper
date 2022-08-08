// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRendererQueueItem.generated.h"

class UMoviePipelineSetting;

UCLASS(Transient)
class UGamedevHelperRendererQueueItem : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperRendererQueueItem();
	
	UPROPERTY()
	EGamedevHelperRendererStatus Status;

	UPROPERTY()
	FString PipelineName;
	
	UPROPERTY()
	FString SequenceName;
	
	//
	// UPROPERTY()
	// FString SequenceDuration;
	//
	// UPROPERTY()
	// int32 SequenceDurationInFrames;
	//
	// UPROPERTY()
	// int32 SequenceStartFrame;
	//
	// UPROPERTY()
	// int32 SequenceRenderedFrames;
	//
	// UPROPERTY()
	// FString SequenceOutputDir;
	//
	UPROPERTY()
	FString Note;
	//
	// UPROPERTY()
	// FSoftObjectPath LevelSequence;
	//
	// UPROPERTY()
	// FSoftObjectPath Map;
	//
	// UPROPERTY()
	// TArray<UMoviePipelineSetting*> UserSettings;
	//
	// bool IsRendered() const;
	// bool IsValid() const;
	// int32 GetRenderedFramesNum() const;
};
