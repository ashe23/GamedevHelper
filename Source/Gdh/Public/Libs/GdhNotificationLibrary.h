// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GdhNotificationLibrary.generated.h"

UCLASS()
class UGdhNotificationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Notification")
	static void ShowModal(const FString& Title, const FString& Msg, const EGdhModalStatus ModalStatus = EGdhModalStatus::None, const float Duration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Notification")
	static void ShowModalOutputLog(const FString& Title, const FString& Msg, const EGdhModalStatus ModalStatus = EGdhModalStatus::None, const float Duration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Notification")
	static void ShowModalWithDirLink(const FString& Title,
	                                 const FString& Msg,
	                                 const FString& DirPath,
	                                 const FString& DirPathText,
	                                 const EGdhModalStatus ModalStatus = EGdhModalStatus::None,
	                                 const float Duration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Notification")
	static void ShowModalWithFileLink(const FString& Title,
	                                  const FString& Msg,
	                                  const FString& FilePath,
	                                  const FString& FilePathText,
	                                  const EGdhModalStatus ModalStatus = EGdhModalStatus::None,
	                                  const float Duration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Notification")
	static EAppReturnType::Type ShowDialogWindow(const FString& Title, const FString& Msg, const EAppMsgType::Type MsgType);
private:
	static SNotificationItem::ECompletionState GetCompletionStateFromModalStatus(const EGdhModalStatus ModalStatus);
};
