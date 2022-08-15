// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "GamedevHelperProjectSettings.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineExecutor.h"
#include "MovieSceneTimeHelpers.h"

UGamedevHelperRenderingManagerQueueSettings::UGamedevHelperRenderingManagerQueueSettings()
{
	
}

#if WITH_EDITOR
void UGamedevHelperRenderingManagerQueueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif
