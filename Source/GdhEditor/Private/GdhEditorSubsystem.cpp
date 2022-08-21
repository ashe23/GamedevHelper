// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhEditorSubsystem.h"
#include "GdhEditor.h"
// Engine Headers
#include "GdhRenderingSettings.h"
#include "LevelEditorSubsystem.h"
#include "LevelSequence.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Subsystems/UnrealEditorSubsystem.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegisterContextMenuActions();
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGdhSubsystem::RenderLevelSequence(const TSoftObjectPtr<ULevelSequence> LevelSequence, const TSoftObjectPtr<UWorld> Map, const bool bCreateVideo)
{
	if (!GEditor) return;

	const ULevelSequence* Sequence = LevelSequence.LoadSynchronous();
	if (!Sequence) return;

	const UWorld* MapAsset = Map.LoadSynchronous();
	if (!MapAsset)
	{
		MapAsset = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>()->GetEditorWorld();
	}

	if (!MapAsset) return;

	UMoviePipelineQueue* CustomQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!CustomQueue) return;

	CustomQueue->DeleteAllJobs();

	UMoviePipelineExecutorJob* Job = CustomQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
	if (!Job) return;

	UGdhRenderingSettings* RenderingSettings = GetMutableDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return;

	RenderingSettings->Validate();
	if (!RenderingSettings->IsValid())
	{
		UE_LOG(LogGdhEditor, Error, TEXT("Invalid settings: %s"), *RenderingSettings->GetErrorMsg());
		return;
	}

	Job->SetConfiguration(RenderingSettings->GetMasterConfig(Sequence));
	Job->Map = MapAsset;
	Job->JobName = Sequence->GetName();
	Job->SetSequence(Sequence);

	UMoviePipelineExecutorBase* Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());
	if (!Executor) return;

	Executor->OnExecutorFinished().AddLambda([](const UMoviePipelineExecutorBase* ExecutorBase, const bool bSuccess)
	{
		if (!bSuccess)
		{
			UE_LOG(LogGdhEditor, Error, TEXT("Error occured when rendering images"));
			ShowModalWithOutputLog(TEXT("GamedevHelper: Rendering Manager"), TEXT("Error Occured when rendering images"), EGdhModalStatus::Error, 5.0f);
			return;
		}

		ShowModal(TEXT("GamedevHelper: Rendering Manager"), TEXT("Rendering images finished successfully"), EGdhModalStatus::OK, 5.0f);
	});
}

void UGdhSubsystem::EncodeLevelSequence(const TSoftObjectPtr<ULevelSequence> LevelSequence)
{
	const ULevelSequence* Sequence = LevelSequence.LoadSynchronous();
	if (!Sequence) return;

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return;

	// todo:ashe23 check if sequence images already rendered? 

	UE_LOG(LogGdhEditor, Warning, TEXT("%s"), *RenderingSettings->GetEncodeCommand(Sequence));
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
