// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSubsystem.h"
// Engine Headers
#include "MoviePipelineMasterConfig.h"
#include "MovieSceneTimeHelpers.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Kismet/KismetStringLibrary.h"
#include "Settings/GdhRenderingSettings.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UGdhSubsystem::IsValidJobSetting(UMoviePipelineSetting* Setting)
{
	if (!Setting) return false;

	Setting->ValidateState();

	return Setting->GetValidationState() == EMoviePipelineValidationState::Valid;
}

bool UGdhSubsystem::IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig)
{
	if (!MasterConfig) return false;

	for (const auto& Setting : MasterConfig->GetAllSettings())
	{
		if (!IsValidJobSetting(Setting))
		{
			return false;
		}
	}

	return true;
}

FString UGdhSubsystem::GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig)
{
	if (!MasterConfig) return TEXT("");

	for (const auto& Setting : MasterConfig->GetAllSettings())
	{
		Setting->ValidateState();

		if (Setting->GetValidationState() == EMoviePipelineValidationState::Errors || Setting->GetValidationState() == EMoviePipelineValidationState::Warnings)
		{
			const TArray<FText> ValidationResults = Setting->GetValidationResults();

			FString ValidateErrorText;

			for (const auto& ValidationResult : ValidationResults)
			{
				ValidateErrorText.Append(ValidationResult.ToString() + TEXT("\n"));
			}

			return ValidateErrorText;
		}
	}

	return TEXT("");
}

uint32 UGdhSubsystem::GetRenderedFramesNum(const ULevelSequence* LevelSequence, bool& bHasMissingFrames)
{
	if (!LevelSequence) return 0;

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return 0;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	struct DirectoryVisitor : IPlatformFile::FDirectoryVisitor
	{
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory) return false;

			const FString BaseName = FPaths::GetBaseFilename(FilenameOrDirectory);
			const FString Extension = FPaths::GetExtension(FilenameOrDirectory);
			if (BaseName.StartsWith(RequiredBaseName) && Extension.Equals(RequiredExtension))
			{
				TArray<FString> Parts;
				BaseName.ParseIntoArray(Parts, TEXT("_"));
				Frames.Add(FCString::Atoi(*Parts.Last()));
			}

			return true;
		}

		FString RequiredBaseName;
		FString RequiredExtension;
		TArray<int32> Frames;
	};

	DirectoryVisitor Visitor;
	Visitor.RequiredBaseName = LevelSequence->GetName();
	Visitor.RequiredExtension = RenderingSettings->GetImageExtension();

	const FString Path = GetImageOutputDirectoryPath(LevelSequence);
	if (Path.IsEmpty()) return 0;

	if (!PlatformFile.IterateDirectory(*Path, Visitor))
	{
		return 0;
	}

	if (Visitor.Frames.Num() == 0)
	{
		return 0;
	}

	Visitor.Frames.Sort();
	bHasMissingFrames = false;

	for (int32 i = 0; i < Visitor.Frames.Num(); ++i)
	{
		if (Visitor.Frames[i] != i)
		{
			bHasMissingFrames = true;
		}
	}

	return Visitor.Frames.Num();
}

uint32 UGdhSubsystem::GetLevelSequenceDuration(const ULevelSequence* LevelSequence)
{
	if (!LevelSequence) return 0;
	if (!LevelSequence->MovieScene) return 0;

	const int32 FrameStart = ConvertFrameTime(
			UE::MovieScene::DiscreteInclusiveLower(LevelSequence->MovieScene->GetPlaybackRange()),
			LevelSequence->MovieScene->GetTickResolution(),
			LevelSequence->MovieScene->GetDisplayRate()
		).FloorToFrame().Value;
	const int32 FrameEnd = ConvertFrameTime(
		UE::MovieScene::DiscreteExclusiveUpper(LevelSequence->MovieScene->GetPlaybackRange()),
		LevelSequence->MovieScene->GetTickResolution(),
		LevelSequence->MovieScene->GetDisplayRate()
	).FloorToFrame().Value;

	return FrameEnd - FrameStart;
}

FString UGdhSubsystem::GetImageOutputDirectoryPath(const ULevelSequence* LevelSequence)
{
	if (!LevelSequence) return TEXT("");

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return TEXT("");

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/image/sequences/%s/%s"),
			*RenderingSettings->OutputDirectory.Path,
			*RenderingSettings->GetResolutionFolderName(),
			*LevelSequence->GetName()
		)
	);
}

FString UGdhSubsystem::GetVideoOutputDirectoryPath(const ULevelSequence* LevelSequence)
{
	if (!LevelSequence) return TEXT("");

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return TEXT("");

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/video/sequences/%s/%s"),
			*RenderingSettings->OutputDirectory.Path,
			*RenderingSettings->GetResolutionFolderName(),
			*LevelSequence->GetName()
		)
	);
}

void UGdhSubsystem::ShowModal(const FString& Msg, const EGdhModalStatus ModalStatus, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.ExpireDuration = Duration;
	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
	}
}

void UGdhSubsystem::ShowModalWithOutputLog(const FString& Msg, const EGdhModalStatus ModalStatus, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateStatic([]() { FGlobalTabmanager::Get()->TryInvokeTab(FName("OutputLog")); });
	Info.HyperlinkText = NSLOCTEXT("GamedevHelperShowOutputLog", "GamedevHelperShowOutputLog", "Show OutputLog...");
	const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Notification.Get()->SetCompletionState(GetCompletionStateFromModalStatus(ModalStatus));
	}
}

SNotificationItem::ECompletionState UGdhSubsystem::GetCompletionStateFromModalStatus(const EGdhModalStatus ModalStatus)
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
