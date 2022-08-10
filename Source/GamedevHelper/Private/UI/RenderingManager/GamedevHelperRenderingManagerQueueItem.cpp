// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueItem.h"

UGamedevHelperRenderingManagerQueueItem::UGamedevHelperRenderingManagerQueueItem()
{
	Status = EGamedevHelperRendererStatus::None;
	
}

bool UGamedevHelperRenderingManagerQueueItem::IsRendered() const
{
	return false;
}

bool UGamedevHelperRenderingManagerQueueItem::IsValid() const
{
	return Status != EGamedevHelperRendererStatus::Error;
}

int32 UGamedevHelperRenderingManagerQueueItem::GetRenderedFramesNum() const
{
	return 0;
}

void UGamedevHelperRenderingManagerQueueItem::Validate(const FGamedevHelperRenderingManagerQueueItemData& QueueItemData)
{
	Data = QueueItemData;

	const UMoviePipelineQueue* MoviePipelineQueue = Cast<UMoviePipelineQueue>(QueueItemData.QueueAsset.TryLoad());
	if (!MoviePipelineQueue)
	{
		Note = GamedevHelperStandardText::ErrInvalidQueue;
		Status = EGamedevHelperRendererStatus::Error;
		return;
	}

	// get all jobs in queue
	const TArray<UMoviePipelineExecutorJob*> Jobs = MoviePipelineQueue->GetJobs();
	if (Jobs.Num() == 0)
	{
		Note = GamedevHelperStandardText::ErrNoJobsInQueue;
		Status = EGamedevHelperRendererStatus::Error;
		return;
	}

	
	
	Note = "";
}
