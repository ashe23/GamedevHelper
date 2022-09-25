// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Libs/GdhRenderingLibrary.h"
#include "Libs/GdhNotificationLibrary.h"
// Engine Headers
#include "IPythonScriptPlugin.h"
#include "MoviePipelineMasterConfig.h"
#include "MoviePipelineQueue.h"
#include "MovieSceneTimeHelpers.h"
#include "Async/Async.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Settings/GdhRenderingSettings.h"

bool UGdhRenderingLibrary::IsValidJobSetting(UMoviePipelineSetting* Setting)
{
	if (!Setting) return false;

	Setting->ValidateState();

	return Setting->GetValidationState() == EMoviePipelineValidationState::Valid;
}

bool UGdhRenderingLibrary::IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig)
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

FString UGdhRenderingLibrary::GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig)
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

			return ValidateErrorText.TrimStartAndEnd();
		}
	}

	return TEXT("");
}

uint32 UGdhRenderingLibrary::GetRenderedFramesNum(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue, bool& bHasMissingFrames)
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
			const FString RequiredName = FString::Printf(TEXT("%s_%s"), *RequiredBaseName, *RequiredFramerate);
			if (BaseName.StartsWith(RequiredName) && Extension.Equals(RequiredExtension))
			{
				TArray<FString> Parts;
				BaseName.ParseIntoArray(Parts, TEXT("_"));
				Frames.Add(FCString::Atoi(*Parts.Last()));
			}

			return true;
		}

		FString RequiredBaseName;
		FString RequiredExtension;
		FString RequiredFramerate;
		TArray<int32> Frames;
	};

	DirectoryVisitor Visitor;
	Visitor.RequiredBaseName = LevelSequence->GetName();
	Visitor.RequiredExtension = RenderingSettings->GetImageExtension();
	Visitor.RequiredFramerate = FString::Printf(TEXT("%.3f"), RenderingSettings->Framerate.AsDecimal());

	const FString Path = GetImageOutputDirectoryPath(LevelSequence, MoviePipelineQueue);
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

uint32 UGdhRenderingLibrary::GetLevelSequenceDuration(const ULevelSequence* LevelSequence)
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

FString UGdhRenderingLibrary::GetImageOutputDirectoryPath(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue)
{
	if (!LevelSequence) return TEXT("");

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return TEXT("");

	if (MoviePipelineQueue)
	{
		return FPaths::ConvertRelativePathToFull(
			FString::Printf(
				TEXT("%s/%s/%s/%s/image"),
				*RenderingSettings->OutputDirectory.Path,
				*MoviePipelineQueue->GetName(),
				*LevelSequence->GetName(),
				*RenderingSettings->GetResolutionFolderName()
			)
		);
	}

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/%s/%s/image"),
			*RenderingSettings->OutputDirectory.Path,
			*LevelSequence->GetName(),
			*RenderingSettings->GetResolutionFolderName()
		)
	);
}

FString UGdhRenderingLibrary::GetVideoOutputDirectoryPath(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue)
{
	if (!LevelSequence) return TEXT("");

	const UGdhRenderingSettings* RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return TEXT("");

	if (MoviePipelineQueue)
	{
		return FPaths::ConvertRelativePathToFull(
			FString::Printf(
				TEXT("%s/%s/%s/%s/video"),
				*RenderingSettings->OutputDirectory.Path,
				*MoviePipelineQueue->GetName(),
				*LevelSequence->GetName(),
				*RenderingSettings->GetResolutionFolderName()
			)
		);
	}

	return FPaths::ConvertRelativePathToFull(
		FString::Printf(
			TEXT("%s/%s/%s/video"),
			*RenderingSettings->OutputDirectory.Path,
			*LevelSequence->GetName(),
			*RenderingSettings->GetResolutionFolderName()
		)
	);
}

FString UGdhRenderingLibrary::GetFFmpegEncodeCmd(const ULevelSequence* LevelSequence, const UMoviePipelineQueue* MoviePipelineQueue)
{
	if (!LevelSequence) return TEXT("");

	const auto RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return TEXT("");

	return FString::Printf(
		TEXT("%s -y -framerate %.3f -i %s/%s_%0.3f_%%04d.%s -vf scale=%d:%d %s %s/%s.%s"),
		*RenderingSettings->FFmpegExe.FilePath,
		RenderingSettings->Framerate.AsDecimal(),
		*GetImageOutputDirectoryPath(LevelSequence, MoviePipelineQueue),
		*LevelSequence->GetName(),
		RenderingSettings->Framerate.AsDecimal(),
		*RenderingSettings->GetImageExtension(),
		RenderingSettings->GetResolution().X,
		RenderingSettings->GetResolution().Y,
		*UKismetStringLibrary::JoinStringArray(RenderingSettings->FFmpegFlags, TEXT(" ")),
		*GetVideoOutputDirectoryPath(LevelSequence, MoviePipelineQueue),
		*LevelSequence->GetName(),
		*RenderingSettings->GetVideoExtension()
	);
}

void UGdhRenderingLibrary::RunFFmpegCommands(const TArray<FGdhFFmpegCommand>& FFmpegCommands)
{
	if (FFmpegCommands.Num() == 0) return;

	const auto RenderingSettings = GetDefault<UGdhRenderingSettings>();
	if (!RenderingSettings) return;

	const FString JsonFilePath = FString::Printf(TEXT("%s/ffmpeg_commands.json"), *RenderingSettings->OutputDirectory.Path);
	const TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());
	for (const auto& FFmpegCommand : FFmpegCommands)
	{
		const FString PipelineFieldName = FString::Printf(TEXT("%s"), *FFmpegCommand.SequenceName);
		RootObject->SetStringField(PipelineFieldName, FFmpegCommand.Command);
	}

	FString JsonStr;
	const TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);

	if (!FFileHelper::SaveStringToFile(JsonStr, *JsonFilePath))
	{
		const FString ErrMsg = FString::Printf(TEXT("Failed to export %s file"), *JsonFilePath);
		UE_LOG(LogGdh, Warning, TEXT("%s"), *ErrMsg);
		UGdhNotificationLibrary::ShowModal(TEXT("Rendering Manager"), ErrMsg, EGdhModalStatus::Error, 5.0f);
		return;
	}

	// todo:ashe23 add encoding progress modal
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [JsonFilePath]()
	{
		const FString PythonCmd = FString::Printf(TEXT("ffmpeg_cli_background.py -queue %s"), *JsonFilePath);
		IPythonScriptPlugin::Get()->ExecPythonCommand(*PythonCmd);
	});
}
