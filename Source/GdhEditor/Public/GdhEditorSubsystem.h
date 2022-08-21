// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhCoreTypes.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GdhEditorSubsystem.generated.h"

class ULevelSequence;
class UMoviePipeline;
class UMoviePipelineExecutorBase;

UCLASS(DisplayName="GamedevHelper Subsystem")
class UGdhSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	static void RenderingManagerStartRender();
	static void RenderingManagerStartEncode(const TArray<FGdhFFmpegCommand>& FFmpegCommands);

	// Notification library

	UFUNCTION(BlueprintCallable, Category="GDH|NotificationLibrary")
	static void ShowModal(const FString& Msg, const FString& SubMsg = TEXT(""), const EGdhModalStatus Status = EGdhModalStatus::None, const float Duration = 2.0f);

	UFUNCTION(BlueprintCallable, Category="GDH|NotificationLibrary")
	static void ShowModalWithOutputLog(const FString& Msg, const FString& SubMsg = TEXT(""), const EGdhModalStatus Status = EGdhModalStatus::None, const float Duration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category="GDH|NotificationLibrary")
	static void ShowModalWithOpenDirLink(const FString& Directory, const FString& DirectoryLinkText, const FString& Msg, const FString& SubMsg = TEXT(""), const EGdhModalStatus Status = EGdhModalStatus::None, const float Duration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category="GDH|NotificationLibrary")
	static void ShowModalWithOpenFileLink(const FString& File, const FString& FileLinkText, const FString& Msg, const FString& SubMsg = TEXT(""), const EGdhModalStatus Status = EGdhModalStatus::None, const float Duration = 2.0f);
private:
	void RegisterContextMenuActions() const;
	static SNotificationItem::ECompletionState GetCompletionStateFromModalStatus(const EGdhModalStatus Status);
};