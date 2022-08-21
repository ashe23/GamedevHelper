// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorSubsystem.h"
#include "GdhEditor.h"
#include "GdhRenderingQueueSettings.h"
#include "GdhRenderingSettings.h"
// Engine Headers
#include "LevelSequence.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "IPythonScriptPlugin.h"
#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegisterContextMenuActions();
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGdhSubsystem::RenderingManagerStartRender()
{
	UGdhRenderingSettings* RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	UGdhRenderingQueueSettings* RenderingQueueSettings = GetMutableDefault<UGdhRenderingQueueSettings>();

	check(RenderingSettings);
	check(RenderingQueueSettings);

	RenderingSettings->Validate();
	RenderingQueueSettings->Validate();

	if (!RenderingSettings->IsValid() || !RenderingQueueSettings->IsValid()) return;
	if (!GEditor) return;


	const UWorld* MapAsset = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
	if (!MapAsset) return;

	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue) return;

	CustomQueue->DeleteAllJobs();

	for (const auto& LevelSequence : RenderingQueueSettings->LevelSequences)
	{
		UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
		if (!Job) return;

		Job->SetConfiguration(RenderingSettings->GetMasterConfig(LevelSequence.Get()));
		Job->Map = MapAsset;
		Job->JobName = LevelSequence->GetName();
		Job->SetSequence(LevelSequence.Get());
	}

	for (const auto& MoviePipelineQueue : RenderingQueueSettings->MoviePipelineQueues)
	{
		for (const auto& QueueJob : MoviePipelineQueue->GetJobs())
		{
			UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
			if (!Job) return;

			const ULevelSequence* Sequence = Cast<ULevelSequence>(QueueJob->Sequence.TryLoad());
			if (!Sequence) return;

			Job->SetConfiguration(RenderingSettings->GetMasterConfig(Sequence, MoviePipelineQueue.Get()));
			Job->Map = QueueJob->Map;
			Job->JobName = QueueJob->JobName;
			Job->SetSequence(QueueJob->Sequence);
		}
	}

	UMoviePipelineExecutorBase* Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	if (!Executor) return;

	Executor->OnExecutorFinished().AddLambda([](const UMoviePipelineExecutorBase* ExecutorBase, const bool bSuccess)
	{
		if (!bSuccess)
		{
			ShowModalWithOutputLog(TEXT("GamedevHelper: Rendering Manager"), TEXT("Error Occured when rendering images"), EGdhModalStatus::Error, 5.0f);
			return;
		}

		ShowModal(TEXT("GamedevHelper: Rendering Manager"), TEXT("Rendering images finished successfully"), EGdhModalStatus::OK, 5.0f);

		TArray<FGdhFFmpegCommand> FFmpegCommands;
		GetMutableDefault<UGdhRenderingQueueSettings>()->GetFFmpegCommands(FFmpegCommands);
		
		RenderingManagerStartEncode(FFmpegCommands);
	});
	Executor->OnExecutorErrored().AddLambda([](const UMoviePipelineExecutorBase* PipelineExecutor, const UMoviePipeline* PipelineWithError, const bool bIsFatal, const FText ErrorText)
	{
		const FString ErrorMsg = FString::Printf(TEXT("Error occured with msg: %s"), *ErrorText.ToString());
		ShowModalWithOutputLog(TEXT("GamedevHelper: Rendering Manager"), ErrorMsg, EGdhModalStatus::Error, 5.0f);
	});
}

void UGdhSubsystem::RenderingManagerStartEncode(const TArray<FGdhFFmpegCommand>& FFmpegCommands)
{
	if (FFmpegCommands.Num() == 0) return;

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	check(RenderingSettings);

	const FString JsonFilePath = RenderingSettings->GetOutputDirectory() + TEXT("/") + GdhEditorConstants::FFmpegJsonFile;

	const TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());
	for (const auto& FFmpegCommand : FFmpegCommands)
	{
		const FString PipelineFieldName = FString::Printf(TEXT("%s:%s:%s:%s"), *FFmpegCommand.CommandTitle, *FFmpegCommand.QueueName, *FFmpegCommand.SequenceName, *FFmpegCommand.AudioTrack);
		RootObject->SetStringField(PipelineFieldName, FFmpegCommand.Command);
	}

	FString JsonStr;
	const TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);

	if (!FFileHelper::SaveStringToFile(JsonStr, *JsonFilePath))
	{
		const FString ErrMsg = FString::Printf(TEXT("Failed to export %s file"), *JsonFilePath);
		ShowModal(TEXT("GamedevHelper: Rendering Manager"), ErrMsg, EGdhModalStatus::Error, 5.0f);
		UE_LOG(LogGdhEditor, Warning, TEXT("%s"), *ErrMsg);
		return;
	}

	const FString PythonCmd = FString::Printf(TEXT("%s -queue %s"), *GdhEditorConstants::FFmpegPythonScript, *JsonFilePath);
	IPythonScriptPlugin::Get()->ExecPythonCommand(*PythonCmd);
}

void UGdhSubsystem::ShowModal(const FString& Msg, const FString& SubMsg, const EGdhModalStatus Status, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.ExpireDuration = Duration;
	Info.SubText = FText::FromString(SubMsg);

	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(Status));
	}
}

void UGdhSubsystem::ShowModalWithOutputLog(const FString& Msg, const FString& SubMsg, const EGdhModalStatus Status, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.ExpireDuration = Duration;
	Info.SubText = FText::FromString(SubMsg);
	Info.Hyperlink = FSimpleDelegate::CreateLambda([]()
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FName{TEXT("OutputLog")});
	});
	Info.HyperlinkText = FText::FromString(TEXT("Show Output Log..."));

	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(Status));
	}
}

void UGdhSubsystem::ShowModalWithOpenDirLink(const FString& Directory, const FString& DirectoryLinkText, const FString& Msg, const FString& SubMsg, const EGdhModalStatus Status, const float Duration)
{
	if (Directory.IsEmpty() || !FPaths::DirectoryExists(Directory))
	{
		UE_LOG(LogGdhEditor, Error, TEXT("Directory %s does not exist"), *Directory);
		return;
	}

	FNotificationInfo Info{FText::FromString(Msg)};
	Info.ExpireDuration = Duration;
	Info.SubText = FText::FromString(SubMsg);
	Info.Hyperlink = FSimpleDelegate::CreateLambda([&](const FString& Dir)
	{
		FPlatformProcess::ExploreFolder(*FPaths::ConvertRelativePathToFull(Dir));
	}, Directory);
	Info.HyperlinkText = FText::FromString(DirectoryLinkText);

	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(Status));
	}
}

void UGdhSubsystem::ShowModalWithOpenFileLink(const FString& File, const FString& FileLinkText, const FString& Msg, const FString& SubMsg, const EGdhModalStatus Status, const float Duration)
{
	if (File.IsEmpty() || !FPaths::FileExists(File))
	{
		UE_LOG(LogGdhEditor, Error, TEXT("File %s does not exist"), *File);
		return;
	}

	FNotificationInfo Info{FText::FromString(Msg)};
	Info.ExpireDuration = Duration;
	Info.SubText = FText::FromString(SubMsg);
	Info.Hyperlink = FSimpleDelegate::CreateLambda([&](const FString& FilePath)
	{
		FPlatformProcess::LaunchFileInDefaultExternalApplication(*FPaths::ConvertRelativePathToFull(FilePath));
	}, File);
	Info.HyperlinkText = FText::FromString(File);

	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(Status));
	}
}

void UGdhSubsystem::RegisterContextMenuActions() const
{
	// todo:ashe23
}

SNotificationItem::ECompletionState UGdhSubsystem::GetCompletionStateFromModalStatus(const EGdhModalStatus Status)
{
	switch (Status)
	{
		case EGdhModalStatus::Pending:
			return SNotificationItem::CS_Pending;
		case EGdhModalStatus::Error:
			return SNotificationItem::CS_Fail;
		case EGdhModalStatus::OK:
			return SNotificationItem::CS_Success;
		case EGdhModalStatus::None:
		default:
			return SNotificationItem::CS_None;
	}
}
