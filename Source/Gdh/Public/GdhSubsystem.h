// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GdhSubsystem.generated.h"

class ULevelSequence;
class UMoviePipelineSetting;
class UMoviePipelineMasterConfig;

UCLASS(DisplayName="GamedevHelper Subsystem")
class UGdhSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// rendering library
	static bool IsValidJobSetting(UMoviePipelineSetting* Setting);
	static bool IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig);
	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
	static uint32 GetRenderedFramesNum(const ULevelSequence* LevelSequence, bool& bHasMissingFrames);
	static uint32 GetLevelSequenceDuration(const ULevelSequence* LevelSequence);
	static FString GetImageOutputDirectoryPath(const ULevelSequence* LevelSequence);
	static FString GetVideoOutputDirectoryPath(const ULevelSequence* LevelSequence);
	// notification library
	static void ShowModal(const FString& Msg, const EGdhModalStatus ModalStatus = EGdhModalStatus::None, const float Duration = 2.0f);
	static void ShowModalWithOutputLog(const FString& Msg,const EGdhModalStatus ModalStatus = EGdhModalStatus::None, const float Duration = 2.0f);
private:
	static SNotificationItem::ECompletionState GetCompletionStateFromModalStatus(const EGdhModalStatus ModalStatus);
};