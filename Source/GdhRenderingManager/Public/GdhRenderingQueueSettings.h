// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhCoreTypes.h"
#include "GdhRenderingQueueSettings.generated.h"

DECLARE_DELEGATE(FGDhQueueSettingsChanged);

class ULevelSequence;
class UMoviePipelineQueue;

UCLASS(DisplayName="Queue Settings")
class GDHRENDERINGMANAGER_API UGdhRenderingQueueSettings : public UObject
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	bool IsValid() const;
	FString GetErrorMsg() const;
	void Validate();

	void GetFFmpegCommands(TArray<FGdhFFmpegCommand>& FFmpegCommands);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QueueSettings")
	TSet<TSoftObjectPtr<ULevelSequence>> LevelSequences;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QueueSettings")
	TSet<TSoftObjectPtr<UMoviePipelineQueue>> MoviePipelineQueues;

	FGDhQueueSettingsChanged QueueSettingsChanged;
	
private:
	FString ErrorMsg;
	bool bIsValid = false;
};