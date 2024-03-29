﻿// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerListItem.h"
#include "GamedevHelperProjectSettings.h"
#include "GamedevHelper.h"
#include "Settings/GamedevHelperRenderingSettings.h"
// Engine Headers
#include "GamedevHelperSubsystem.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineConsoleVariableSetting.h"
#include "MoviePipelineExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MovieSceneTimeHelpers.h"


#if WITH_EDITOR
void UGamedevHelperRenderingManagerQueueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Validate();

	if (IsValid())
	{
		SaveConfig();
	}

	if (QueueSettingsDelegate.IsBound())
	{
		QueueSettingsDelegate.Execute();
	}
}
#endif

FString UGamedevHelperRenderingManagerQueueSettings::GetErrorMsg() const
{
	return ErrorMsg;
}

void UGamedevHelperRenderingManagerQueueSettings::Validate()
{
	if (QueueAssets.Num() == 0)
	{
		ErrorMsg = TEXT("Error: Queue assets not specified");
		return;
	}

	for (const auto& QueueAsset : QueueAssets)
	{
		const TSoftObjectPtr<UMoviePipelineQueue> QueueAssetData = QueueAsset.TryLoad();
		if (!QueueAssetData)
		{
			ErrorMsg = TEXT("Error: Failed to load MoviePipelineQueue asset");
			return;
		}

		const TArray<UMoviePipelineExecutorJob*> QueueJobs = QueueAssetData->GetJobs();
		if (QueueJobs.Num() == 0)
		{
			ErrorMsg = FString::Printf(TEXT("Error: Queue %s is empty. Must have at least 1 job"), *QueueAsset.GetAssetName());
			return;
		}
	}

	ErrorMsg.Reset();
}

bool UGamedevHelperRenderingManagerQueueSettings::IsValid() const
{
	return ErrorMsg.IsEmpty();
}
