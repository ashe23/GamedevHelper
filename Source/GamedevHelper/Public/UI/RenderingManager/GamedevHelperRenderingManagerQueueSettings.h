// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerQueueSettings.generated.h"

UCLASS()
class UGamedevHelperRenderingManagerQueueSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RendererQueue|Queue", DisplayName="QueueAssets", meta=(ToolTip = "MovieRenderPipelineQueue assets"))
	TArray<FGamedevHelperRenderingManagerQueueItemData> Queue;

	bool IsValid();
};