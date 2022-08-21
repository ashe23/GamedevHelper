// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingQueueSettings.h"
// Engine Headers
#include "MoviePipelineQueue.h"
// #include "LevelSequence.h"

#if WITH_EDITOR
void UGdhRenderingQueueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Validate();

	SaveConfig();

	if (QueueSettingsChanged.IsBound())
	{
		QueueSettingsChanged.Execute();
	}
}
#endif

bool UGdhRenderingQueueSettings::IsValid() const
{
	return bIsValid;
}

FString UGdhRenderingQueueSettings::GetErrorMsg() const
{
	return ErrorMsg;
}

void UGdhRenderingQueueSettings::Validate()
{
	if (LevelSequences.Num() == 0 && MoviePipelineQueues.Num() == 0)
	{
		bIsValid = false;
		ErrorMsg = TEXT("Specify assets for rendering");
		return;
	}

	for (const auto& LevelSequence : LevelSequences)
	{
		const ULevelSequence* Sequence = LevelSequence.LoadSynchronous();
		if (!Sequence)
		{
			bIsValid = false;
			ErrorMsg = TEXT("Invalid LevelSequence asset in list");
			return;
		}
	}

	for (const auto& MoviePipelineQueue : MoviePipelineQueues)
	{
		const UMoviePipelineQueue* Queue = MoviePipelineQueue.LoadSynchronous();
		if (!Queue)
		{
			bIsValid = false;
			ErrorMsg = TEXT("Invalid MoviePipelineQueue asset in list");
			return;
		}

		const auto Jobs = Queue->GetJobs();
		if (Jobs.Num() == 0)
		{
			bIsValid = false;
			ErrorMsg = FString::Printf(TEXT("Queue %s has no jobs. At least 1 job required"), *Queue->GetName());
			return;
		}

		for (const auto& Job : Jobs)
		{
			if (!Job)
			{
				bIsValid = false;
				ErrorMsg = FString::Printf(TEXT("Queue %s has invalid job"), *Queue->GetName());
				return;
			}

			if (!Job->Sequence.IsValid())
			{
				bIsValid = false;
				ErrorMsg = FString::Printf(TEXT("Queue %s has job with invalid LevelSequence"), *Queue->GetName());
				return;
			}

			if (!Job->Map.IsValid())
			{
				bIsValid = false;
				ErrorMsg = FString::Printf(TEXT("Queue %s has job with invalid Map"), *Queue->GetName());
				return;
			}
		}
	}

	ErrorMsg.Reset();
	bIsValid = true;
}
