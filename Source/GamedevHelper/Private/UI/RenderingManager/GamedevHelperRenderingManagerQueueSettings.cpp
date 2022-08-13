// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueSettings.h"
#include "GamedevHelperProjectSettings.h"
#include "GamedevHelper.h"
// Engine Headers
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineQueue.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelineExecutor.h"
#include "MovieSceneTimeHelpers.h"

#if WITH_EDITOR
UGamedevHelperRenderingManagerQueueSettings::UGamedevHelperRenderingManagerQueueSettings()
{
	CustomPipeline = NewObject<UMoviePipelineQueue>();
	verify(CustomPipeline);
}

void UGamedevHelperRenderingManagerQueueSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Validate();
	if (IsValid())
	{
		SaveConfig();
	}
}
#endif

bool UGamedevHelperRenderingManagerQueueSettings::IsValid() const
{
	return bIsValid;
}

FString UGamedevHelperRenderingManagerQueueSettings::GetErrorMsg() const
{
	return ErrorMsg;
}

const TObjectPtr<UMoviePipelineQueue>& UGamedevHelperRenderingManagerQueueSettings::GetCustomPipeline() const
{
	return CustomPipeline;
}

const TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>& UGamedevHelperRenderingManagerQueueSettings::GetQueueItems() const
{
	return QueueItems;
}

const TArray<FGamedevHelperFFmpegCommand>& UGamedevHelperRenderingManagerQueueSettings::GetFFmpegCommands() const
{
	return FFmpegCommands;
}

void UGamedevHelperRenderingManagerQueueSettings::Validate()
{
	if (QueueAssets.Num() == 0)
	{
		bIsValid = false;
		ErrorMsg = GamedevHelperStandardText::QueueIsEmpty;
		return;
	}

	const auto RenderingSettings = GetDefault<UGamedevHelperRenderingSettings>();
	verify(RenderingSettings);

	QueueItems.Reset();
	QueueItems.Reserve(QueueAssets.Num());
	CustomPipeline->DeleteAllJobs();

	for (const auto& QueueAsset : QueueAssets)
	{
		const UMoviePipelineQueue* PipelineQueue = Cast<UMoviePipelineQueue>(QueueAsset.TryLoad());
		if (!PipelineQueue)
		{
			bIsValid = false;
			ErrorMsg = GamedevHelperStandardText::ErrInvalidQueue;
			return;
		}

		QueueItems.Reserve(QueueItems.Num() + PipelineQueue->GetJobs().Num());
		
		for (const auto& Job : PipelineQueue->GetJobs())
		{
			const auto QueueItem = NewObject<UGamedevHelperRenderingManagerQueueItem>();
			QueueItem->QueueName = PipelineQueue->GetName();
			QueueItem->Status = EGamedevHelperRendererStatus::OK;
			QueueItem->QueueAsset = QueueAsset;

			UMoviePipelineExecutorJob* CustomJob = CustomPipeline->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
			if (!CustomJob)
			{
				bIsValid = false;
				UE_LOG(LogGamedevHelper, Error, TEXT("Failed to allocate new job"));
				return;
			}

			if (!Job->Sequence.IsValid())
			{
				bIsValid = false;
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				QueueItem->Note = GamedevHelperStandardText::JobMissingLevelSequence;
				QueueItems.Add(QueueItem);
				continue;
			}

			if (!Job->Map.IsValid())
			{
				bIsValid = false;
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				QueueItem->Note = GamedevHelperStandardText::JobMissingMap;
				QueueItems.Add(QueueItem);
				continue;
			}
			
			const ULevelSequence* Sequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			if (!Sequence)
			{
				bIsValid = false;
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				QueueItem->Note = GamedevHelperStandardText::JobCantLoadLevelSequence;
				QueueItems.Add(QueueItem);
				continue;
			}
			
			const auto PlaybackRange = Sequence->MovieScene->GetPlaybackRange();
			const FFrameRate DisplayRate = Sequence->MovieScene->GetDisplayRate();
			const int32 StartFrame = ConvertFrameTime(
						UE::MovieScene::DiscreteInclusiveLower(PlaybackRange),
						Sequence->MovieScene->GetTickResolution(),
						DisplayRate
					).FloorToFrame().Value;
			const int32 EndFrame = ConvertFrameTime(
				UE::MovieScene::DiscreteExclusiveUpper(PlaybackRange),
				Sequence->MovieScene->GetTickResolution(),
				DisplayRate
			).FloorToFrame().Value;

			const int32 DurationInFrames = EndFrame - StartFrame; 
			const double DurationInSeconds = DurationInFrames / RenderingSettings->Framerate.AsDecimal();

			QueueItem->SequenceName = Job->Sequence.GetAssetName();
			QueueItem->SequenceDuration = FString::Printf(TEXT("%d frames (%.2f seconds)"), DurationInFrames, DurationInSeconds);
			QueueItem->SequenceDurationInFrames = DurationInFrames;
			QueueItem->SequenceStartFrame = StartFrame;
			QueueItem->SequenceRenderedFrames = 0; // todo:ashe23


			CustomJob->Sequence = Job->Sequence;
			CustomJob->Map = Job->Map;
			CustomJob->Author = Job->Author;
			CustomJob->ShotInfo = Job->ShotInfo;
			CustomJob->JobName = Job->JobName;

			UMoviePipelineMasterConfig* Config = CustomJob->GetConfiguration();
			if (!Config)
			{
				bIsValid = false;
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				QueueItem->Note = GamedevHelperStandardText::JobInvalidConfigs;
				QueueItems.Add(QueueItem);
				continue;
			}

			Config->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
			Config->FindOrAddSettingByClass(RenderingSettings->GetMoviePipelineOutputSettingImageClass());
			const auto OutputSetting = Cast<UMoviePipelineOutputSetting>(Config->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
			if (!OutputSetting)
			{
				bIsValid = false;
				QueueItem->Status = EGamedevHelperRendererStatus::Error;
				QueueItem->Note = GamedevHelperStandardText::JobInvalidConfigs;
				QueueItems.Add(QueueItem);
				continue;
			}

			OutputSetting->OutputDirectory.Path = RenderingSettings->GetSubDirImage() + TEXT("/") + QueueAsset.GetAssetName();
			OutputSetting->FileNameFormat = RenderingSettings->GetFileNameFormat();
			OutputSetting->OutputResolution = RenderingSettings->GetResolution();
			OutputSetting->bUseCustomFrameRate = true;
			OutputSetting->OutputFrameRate = RenderingSettings->Framerate;
			OutputSetting->bOverrideExistingOutput = true;
			OutputSetting->ZeroPadFrameNumbers = 4;
			OutputSetting->FrameNumberOffset = 0;
			OutputSetting->HandleFrameCount = 0;
			OutputSetting->OutputFrameStep = 1;
			OutputSetting->bUseCustomPlaybackRange = true;
			OutputSetting->CustomStartFrame = StartFrame;
			OutputSetting->CustomEndFrame = EndFrame;
			OutputSetting->PostEditChange();
			
			QueueItems.Add(QueueItem);
		}
	}

	// audio tracks validation
	for (auto& AudioTrack : AudioTracks)
	{
		if (!AudioTrack.Key.IsValid())
		{
			bIsValid = false;
			ErrorMsg = GamedevHelperStandardText::ErrInvalidQueue;
			return;
		}

		AudioTrack.Value.Validate();
		if (!AudioTrack.Value.IsValid())
		{
			bIsValid = false;
			ErrorMsg = AudioTrack.Value.GetErrorMsg();
			return;
		}
	}

	FFmpegCommands.Reset();
	FFmpegCommands.Reserve(QueueItems.Num());

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	// if everything is valid, then we generate ffmpeg commands
	for (const auto& QueueItem : QueueItems)
	{
		// {ffmpeg_exe_path} -y -i {input_image_format} -r {framerate} -vf scale={resolution} {output_video_file}
		// {input_image_format} - {OutputDir}/{ProjectName}/image/{QueueName}/{Sequence}.%04d.{image_format} Example: D:/renders/MyProject/image/MyQueue/MySequence_1920_1080_30.0_%04d.png
		// {output_video_file} - {OutputDir}/{ProjectName}/video/{QueueName}/{Sequence}_{resolution}_{framerate}.{VideoFormat} Example: D:/renders/MyProject/video/MyQueue/MySequence.mp4
		
		const FString InputImageFormat = FString::Printf(
			TEXT("%s/%s/%s_%s_%.1f_%%04d.%s"),
			*RenderingSettings->GetSubDirImage(),
			*QueueItem->QueueName,
			*QueueItem->SequenceName,
			*RenderingSettings->GetResolutionAsString(TEXT("_")),
			RenderingSettings->Framerate.AsDecimal(),
			*RenderingSettings->GetImageFormatAsString()
		);
		
		if (!FPaths::DirectoryExists(FPaths::GetPath(InputImageFormat)))
		{
			PlatformFile.CreateDirectoryTree(*FPaths::GetPath(InputImageFormat));
		}

		const FString OutputVideoFile = FString::Printf(
			TEXT("%s/%s/%s.%s"),
			*RenderingSettings->GetSubDirVideo(),
			*QueueItem->QueueName,
			*QueueItem->SequenceName,
			*RenderingSettings->GetVideoFormatAsString()
		);
		const FString Path = FPaths::GetPath(OutputVideoFile);
		
		if (!FPaths::DirectoryExists(FPaths::GetPath(OutputVideoFile)))
		{
			PlatformFile.CreateDirectoryTree(*FPaths::GetPath(OutputVideoFile));
		}

		const FString ImgToVideoCmd = FString::Printf(
			TEXT("%s -y -i %s -c:v libx264 -x264opts nal-hrd=cbr:force-cfr=1 -pix_fmt yuv420p -r %.1f -vf scale=%s %s"),
			*FPaths::ConvertRelativePathToFull(RenderingSettings->FFmpegExe.FilePath),
			*InputImageFormat,
			RenderingSettings->Framerate.AsDecimal(),
			*RenderingSettings->GetResolutionAsString(),
			*OutputVideoFile
		);

		FFmpegCommands.Add(FGamedevHelperFFmpegCommand{QueueItem->QueueName, QueueItem->SequenceName, TEXT("NoAudio"), ImgToVideoCmd, TEXT("Images to video")});

		const FGamedevHelperAudioTrack* AudioTrack = AudioTracks.Find(QueueItem->QueueAsset);
		if (AudioTrack)
		{
			// {ffmpeg_exe_path} -y -i {input_video_file} -i {input_audio_file} -r {framerate} -map 0 -map 1:a -c:v copy -shortest {output_video_file}
			// {input_video_file} - {OutputDir}/{ProjectName}/video/{QueueName}/{Sequence}_{resolution}_{framerate}.{VideoFormat}
			// {output_video_file} - {OutputDir}/{ProjectName}/mixed/{QueueName}/{AudioTrackName}/{Sequence}_{resolution}_{framerate}.{VideoFormat} Example: D:/renders/MyProject/video/MyQueue/English/MySequence.mp4

			const FString InputVideoFile = OutputVideoFile; // in this stage input is our last output
			
			const FString OutputMixedFile = FString::Printf(
				TEXT("%s/%s/%s/%s.%s"),
				*RenderingSettings->GetSubDirMixed(),
				*QueueItem->QueueName,
				*AudioTrack->Name,
				*QueueItem->SequenceName,
				*RenderingSettings->GetVideoFormatAsString()
			);
			
			if (!FPaths::DirectoryExists(FPaths::GetPath(OutputMixedFile)))
			{
				PlatformFile.CreateDirectoryTree(*FPaths::GetPath(OutputMixedFile));
			}

			const FString VideoAudioMixCmd = FString::Printf(
				TEXT("%s -y -i %s -i %s -r %.1f -map 0 -map 1:a -c:v copy -shortest %s"),
				*FPaths::ConvertRelativePathToFull(RenderingSettings->FFmpegExe.FilePath),
				*InputVideoFile,
				*FPaths::ConvertRelativePathToFull(AudioTrack->Path.FilePath),
				RenderingSettings->Framerate.AsDecimal(),
				*OutputMixedFile
			);

			FFmpegCommands.Add(FGamedevHelperFFmpegCommand{QueueItem->QueueName, QueueItem->SequenceName, AudioTrack->Name, VideoAudioMixCmd, TEXT("Video and Audio mixing")});
		}
	}

	ErrorMsg.Reset();
	bIsValid = true;
}

