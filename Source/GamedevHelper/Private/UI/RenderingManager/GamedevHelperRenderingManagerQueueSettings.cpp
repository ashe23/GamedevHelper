// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineQueue.h"

bool UGamedevHelperRenderingManagerQueueSettings::IsValid()
{
	if (Queue.Num() == 0) return false;

	// for (const FGamedevHelperRenderingManagerQueueItem& QueueItem : Queue)
	// {
	// 	const UMoviePipelineQueue* MoviePipelineQueue = Cast<UMoviePipelineQueue>(QueueItem.QueueAsset.TryLoad());
	// 	if (!MoviePipelineQueue)
	// 	{
	// 		ErrorText = GamedevHelperStandardText::ErrFailedToLoadAsset;
	// 		return false;
	// 	}
	//
	// 	// get all jobs in current queue
	// 	const TArray<UMoviePipelineExecutorJob*> Jobs = MoviePipelineQueue->GetJobs();
	// 	if (Jobs.Num() == 0)
	// 	{
	// 		ErrorText = GamedevHelperStandardText::ErrNoJobsInQueue;
	// 		return false;
	// 	}
	//
	// 	for (const auto& Job : Jobs)
	// 	{
	// 		if (!Job)
	// 		{
	// 			ErrorText = FString::Printf(TEXT("Error: Invalid job in %s asset"), *MoviePipelineQueue->GetName());
	// 			return false;
	// 		}
	//
	// 		if (Job->JobName.IsEmpty())
	// 		{
	// 			// todo:ashe23 not really an error , maybe warning
	// 			ErrorText = FString::Printf(TEXT("Error: Queue %s contains job with empty name"), *MoviePipelineQueue->GetName());
	// 			return false;
	// 		}
	//
	// 		if (!Job->Sequence.IsValid())
	// 		{
	// 			ErrorText = FString::Printf(TEXT("Error: Queue %s constains invalid level sequence"), *MoviePipelineQueue->GetName());
	// 			return false;
	// 		}
	//
	// 		if (!Job->Map.IsValid())
	// 		{
	// 			ErrorText = FString::Printf(TEXT("Error: Queue %s contains invalid map"), *MoviePipelineQueue->GetName());
	// 			return false;
	// 		}
	//
	// 		UMoviePipelineMasterConfig* MasterConfig = Job->GetConfiguration();
	// 		if (!MasterConfig)
	// 		{
	// 			ErrorText = FString::Printf(TEXT("Error: Queue %s contains invalid configs"), *MoviePipelineQueue->GetName());
	// 			return false;
	// 		}
	// 		
	// 	}
	// }
	//
	// ErrorText = TEXT("");

	return true;
}
