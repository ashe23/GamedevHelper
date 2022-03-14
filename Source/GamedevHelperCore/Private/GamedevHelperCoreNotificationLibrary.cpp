// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperCoreNotificationLibrary.h"
// Engine Headers
#include "Framework/Notifications/NotificationManager.h"


void UGamedevHelperNotificationLibrary::ShowModal(const FString& Msg, const EGDHModalStatus ModalStatus, const float Duration)
{
	const FText NotificationText = FText::FromString(Msg);
	FNotificationInfo Info{NotificationText};
	Info.ExpireDuration = Duration;
	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
	}
}

void UGamedevHelperNotificationLibrary::ShowModalWithOutputLog(const FString& Msg, const float Duration)
{
	const FText NotificationText = FText::FromString(Msg);
	FNotificationInfo Info{NotificationText};
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateStatic([]() { FGlobalTabmanager::Get()->TryInvokeTab(FName("OutputLog")); });
	Info.HyperlinkText = NSLOCTEXT("GamedevHelperShowOutputLog", "GamedevHelperShowOutputLog", "Show OutputLog...");
	FSlateNotificationManager::Get().AddNotification(Info);
}

void UGamedevHelperNotificationLibrary::ShowModalWithHyperLink(const FString& Msg, const FString& Link, const float Duration)
{
	const FText NotificationText = FText::FromString(Msg);
	FNotificationInfo Info{NotificationText};
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateStatic([](FString InDirectory) { FPlatformProcess::ExploreFolder(*InDirectory); }, Link);
	Info.HyperlinkText = NSLOCTEXT("VirtualGamesOpenFolder", "VirtualGamesOpenFolder", "Open Folder...");
	FSlateNotificationManager::Get().AddNotification(Info);
}

SNotificationItem::ECompletionState UGamedevHelperNotificationLibrary::GetCompletionStateFromModalStatus(const EGDHModalStatus ModalStatus)
{
	if (ModalStatus == EGDHModalStatus::Pending)
	{
		return SNotificationItem::CS_Pending;
	}
	if (ModalStatus == EGDHModalStatus::Fail)
	{
		return SNotificationItem::CS_Fail;
	}
	if (ModalStatus == EGDHModalStatus::Success)
	{
		return SNotificationItem::CS_Success;
	}

	return SNotificationItem::CS_None;
}
