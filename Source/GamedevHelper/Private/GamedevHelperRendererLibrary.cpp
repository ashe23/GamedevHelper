// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperRendererLibrary.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineImageSequenceOutput.h"
#include "MoviePipelineNewProcessExecutor.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineQueueSubsystem.h"
#include "MovieSceneTimeHelpers.h"

void UGamedevHelperRendererLibrary::RenderLevelSequence(const TSoftObjectPtr<ULevelSequence> SequencePath, const TSoftObjectPtr<UWorld> MapPath)
{
	ULevelSequence* LevelSequence = Cast<ULevelSequence>(SequencePath.ToSoftObjectPath().TryLoad());
	if (!LevelSequence)
	{
		return;
	}
	
	// const UGamedevHelperRendererSettings* RendererSettings = GetDefault<UGamedevHelperRendererSettings>();
	// if (!RendererSettings)
	// {
	// 	return;
	// }
	//
	// if (!GEditor)
	// {
	// 	return;
	// }
	//
	// UMoviePipelineQueue* CustomPipelineQueue = NewObject<UMoviePipelineQueue>();
	// if (!CustomPipelineQueue) return;
	//
	// UMoviePipelineExecutorJob* Job = CustomPipelineQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
	// if (!Job)
	// {
	// 	return;
	// }
	//
	// Job->Sequence = SequencePath.ToSoftObjectPath();
	// Job->Map = MapPath.ToSoftObjectPath();
	// Job->JobName = FString::Printf(TEXT("%s"), *Job->Sequence.GetAssetName());
	//
	// const ULevelSequence* Sequence = Cast<ULevelSequence>(SequencePath.ToSoftObjectPath().TryLoad());
	// const auto PlaybackRange = Sequence->MovieScene->GetPlaybackRange();
	// const FFrameRate DisplayRate = Sequence->MovieScene->GetDisplayRate();
	// const int32 StartFrame = ConvertFrameTime(
	// 			UE::MovieScene::DiscreteInclusiveLower(PlaybackRange),
	// 			Sequence->MovieScene->GetTickResolution(),
	// 			DisplayRate
	// 		).FloorToFrame().Value;
	// const int32 EndFrame = ConvertFrameTime(
	// 	UE::MovieScene::DiscreteExclusiveUpper(PlaybackRange),
	// 	Sequence->MovieScene->GetTickResolution(),
	// 	DisplayRate
	// ).FloorToFrame().Value;
	//
	// UMoviePipelineMasterConfig* Config = Job->GetConfiguration();
	// Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
	// Config->FindOrAddSettingByClass(UMoviePipelineImageSequenceOutput_PNG::StaticClass());
	// const auto OutputSetting = Cast<UMoviePipelineOutputSetting>(Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
	// OutputSetting->OutputDirectory.Path = FString::Printf(TEXT("%s/image/"), *RendererSettings->OutputDir.Path); // todo:ashe23 change later
	// OutputSetting->FileNameFormat = TEXT("{sequence_name}_{output_resolution}_{frame_number_rel}");
	// OutputSetting->OutputResolution = RendererSettings->GetResolution();
	// OutputSetting->bUseCustomFrameRate = true;
	// OutputSetting->OutputFrameRate = RendererSettings->Framerate;
	// OutputSetting->bOverrideExistingOutput = true;
	// OutputSetting->ZeroPadFrameNumbers = 4;
	// OutputSetting->FrameNumberOffset = 0;
	// OutputSetting->HandleFrameCount = 0;
	// OutputSetting->OutputFrameStep = 1;
	// OutputSetting->bUseCustomPlaybackRange = true;
	// OutputSetting->CustomStartFrame = StartFrame;
	// OutputSetting->CustomEndFrame = EndFrame;
	// OutputSetting->PostEditChange();
	//
	// UMoviePipelineQueue* PipelineQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	// PipelineQueue->CopyFrom(CustomPipelineQueue);
	//
	// const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	// Executor->OnExecutorFinished().AddLambda([](UMoviePipelineExecutorBase* ExecutorBase, bool bSuccess)
	// {
	// 	UE_LOG(LogGamedevHelper, Warning, TEXT("Executor finished with error"));
	// });
	// Executor->OnExecutorErrored().AddLambda([](UMoviePipelineExecutorBase* PipelineExecutor, UMoviePipeline* PipelineWithError, bool bIsFatal, FText ErrorText)
	// {
	// 	UE_LOG(LogGamedevHelper, Warning, TEXT("Executor finished rendering. %s"), *ErrorText.ToString());
	// });
	//
	//
	// // ffmpeg -y -i {img_format}.png {output_name}.{video_format}
	// UE_LOG(LogGamedevHelper, Warning, TEXT("Rendering..."));
}
