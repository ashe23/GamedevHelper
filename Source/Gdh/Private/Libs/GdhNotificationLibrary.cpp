// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Libs/GdhNotificationLibrary.h"
// Engine Headers
#include "Gdh.h"
#include "Framework/Notifications/NotificationManager.h"

void UGdhNotificationLibrary::ShowModal(const FString& Title, const FString& Msg, const EGdhModalStatus ModalStatus, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Title)};
	Info.SubText = FText::FromString(Msg);
	Info.ExpireDuration = Duration;

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
}

void UGdhNotificationLibrary::ShowModalOutputLog(const FString& Title, const FString& Msg, const EGdhModalStatus ModalStatus, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Title)};
	Info.SubText = FText::FromString(Msg);
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateLambda([]()
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FName{TEXT("OutputLog")});
	});
	Info.HyperlinkText = FText::FromString(TEXT("Show OutputLog..."));

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
}

void UGdhNotificationLibrary::ShowModalWithDirLink(const FString& Title,
                                                   const FString& Msg,
                                                   const FString& DirPath,
                                                   const FString& DirPathText,
                                                   const EGdhModalStatus ModalStatus,
                                                   const float Duration)
{
	if (DirPath.IsEmpty() || !FPaths::DirectoryExists(DirPath))
	{
		UE_LOG(LogGdh, Error, TEXT("Directory %s does not exist"), *DirPath);
		return;
	}

	FNotificationInfo Info{FText::FromString(Title)};
	Info.SubText = FText::FromString(Msg);
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateLambda([](const FString& DirPath)
	{
		FPlatformProcess::ExploreFolder(*FPaths::ConvertRelativePathToFull(*DirPath));
	}, DirPath);
	Info.HyperlinkText = FText::FromString(DirPathText);

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
}

void UGdhNotificationLibrary::ShowModalWithFileLink(const FString& Title,
                                                    const FString& Msg,
                                                    const FString& FilePath,
                                                    const FString& FilePathText,
                                                    const EGdhModalStatus ModalStatus,
                                                    const float Duration)
{
	if (FilePath.IsEmpty() || !FPaths::FileExists(FilePath))
	{
		UE_LOG(LogGdh, Error, TEXT("File %s does not exist"), *FilePath);
		return;
	}

	FNotificationInfo Info{FText::FromString(Title)};
	Info.SubText = FText::FromString(Msg);
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateLambda([](const FString& FilePath)
	{
		FPlatformProcess::LaunchFileInDefaultExternalApplication(*FPaths::ConvertRelativePathToFull(FilePath));
	}, FilePath);
	Info.HyperlinkText = FText::FromString(FilePathText);

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
}

EAppReturnType::Type UGdhNotificationLibrary::ShowDialogWindow(const FString& Title, const FString& Msg, const EAppMsgType::Type MsgType)
{
	const FText DialogTitle = FText::FromString(Title);
	const FText DialogContent = FText::FromString(Msg);

	return FMessageDialog::Open(MsgType, DialogContent, &DialogTitle);
}

SNotificationItem::ECompletionState UGdhNotificationLibrary::GetCompletionStateFromModalStatus(const EGdhModalStatus ModalStatus)
{
	if (ModalStatus == EGdhModalStatus::Pending)
	{
		return SNotificationItem::CS_Pending;
	}
	if (ModalStatus == EGdhModalStatus::Error)
	{
		return SNotificationItem::CS_Fail;
	}
	if (ModalStatus == EGdhModalStatus::OK)
	{
		return SNotificationItem::CS_Success;
	}

	return SNotificationItem::CS_None;
}
