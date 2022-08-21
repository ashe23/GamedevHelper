// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhCoreTypes.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GdhEditorSubsystem.generated.h"

class ULevelSequence;

UCLASS(DisplayName="GamedevHelper Subsystem")
class UGdhSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	UFUNCTION(BlueprintCallable, Category="GDH|RenderingLibrary", meta=(ToolTip="Renders specified LevelSequence using MovieRender and Gdh Rendering Settings. If Map not specified, will use current Editor Level"))
	static void RenderLevelSequence(const TSoftObjectPtr<ULevelSequence> LevelSequence, const TSoftObjectPtr<UWorld> Map, const bool bCreateVideo);

	UFUNCTION(BlueprintCallable, Category="GDH|RenderingLibrary")
	static void EncodeLevelSequence(const TSoftObjectPtr<ULevelSequence> LevelSequence);

public:
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