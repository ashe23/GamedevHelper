// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhRenderingQueueSettings.generated.h"

class ULevelSequence;
class UMoviePipelineQueue;

UCLASS(Config = EditorPerProjectUserSettings, meta=(DisplayName="Rendering Queue Settings"))
class UGdhRenderingQueueSettings : public UObject
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="QueueSettings")
	TArray<FGdhLevelSequenceRenderSettings> LevelSequences;
};