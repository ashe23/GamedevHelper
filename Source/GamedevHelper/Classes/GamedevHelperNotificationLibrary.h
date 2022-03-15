// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperNotificationLibrary.generated.h"

UCLASS()
class UGamedevHelperNotificationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Notification")
	static void ShowModal(const FString& Msg, const EGamedevHelperModalStatus ModalStatus, const float Duration = 2.0f);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Notification")
	static void ShowModalWithOutputLog(const FString& Msg, const float Duration = 2.0f);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Notification")
	static void ShowModalWithHyperLink(const FString& Msg, const FString& Link, const float Duration = 2.0f);

private:
	static SNotificationItem::ECompletionState GetCompletionStateFromModalStatus(const EGamedevHelperModalStatus ModalStatus);
};
