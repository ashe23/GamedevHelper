// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineQueue.h"

bool UGamedevHelperRenderingManagerQueueSettings::IsValid()
{
	if (Queue.Num() == 0)
	{
		ErrorText = GamedevHelperStandardText::QueueIsEmpty;
		return false;
	}

	for (const auto& QueueItem : Queue)
	{
		const UMoviePipelineQueue* MoviePipelineQueue = Cast<UMoviePipelineQueue>(QueueItem.QueueAsset.TryLoad());
		if (!MoviePipelineQueue)
		{
			ErrorText = GamedevHelperStandardText::ErrInvalidQueue;
			return false;
		}
	}
	
	ErrorText = TEXT("");

	return true;
}

FString UGamedevHelperRenderingManagerQueueSettings::GetErrorText() const
{
	return ErrorText;
}
