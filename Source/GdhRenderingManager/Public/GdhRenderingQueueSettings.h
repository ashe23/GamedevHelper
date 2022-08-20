// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhRenderingQueueSettings.generated.h"

class ULevelSequence;
class UMoviePipelineQueue;

UCLASS(DisplayName="Queue Settings")
class GDHRENDERINGMANAGER_API UGdhRenderingQueueSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QueueSettings")
	TSet<TSoftObjectPtr<ULevelSequence>> LevelSequences;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QueueSettings")
	TSet<TSoftObjectPtr<UMoviePipelineQueue>> MoviePipelineQueues;
};