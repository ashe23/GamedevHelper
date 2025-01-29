// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "VideoEncoderTool/Slate/SGdhVideoEncoderTool.h"
#include "VideoEncoderTool/GdhVideoEncoderToolSettings.h"
#include "GdhCmds.h"
#include "GdhLibEditor.h"
#include "GdhLibPath.h"
#include "GdhToolsModule.h"
#include "CustomAssets/GdhVideoPipeline.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "MoviePipelineConfigBase.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineImageSequenceOutput.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"

void SGdhVideoEncoderTool::Construct(const FArguments& InArgs) {

	VideoEncoderToolSettings = GetMutableDefault<UGdhVideoEncoderToolSettings>();
	if (!VideoEncoderToolSettings.IsValid()) return;

	CmdsRegister();

	FPropertyEditorModule& PropertyEditor = UGdhLibEditor::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhVideoEncoderToolSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(VideoEncoderToolSettings.Get());

	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot().Value(0.4f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SettingsProperty
				]
			]
			+ SSplitter::Slot().Value(0.6f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					CreateToolbarMain()
				]
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SNew(SSeparator).Thickness(3.0f)
				]
				+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0f)
				[
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						// SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhActorNamingToolListItem>>)
						// .ListItemsSource(&ListItems)
						// .ClearSelectionOnClick(true)
						// .SelectionMode(ESelectionMode::Multi)
						// .OnGenerateRow(this, &SGdhActorNamingTool::OnGenerateRow)
						// .HeaderRow(GetHeaderRow())
						SNew(STextBlock).Text(FText::FromName(TEXT("TODO: table here")))
					]
				]
			]
		]
	];
	// clang-format on
}

void SGdhVideoEncoderTool::CmdsRegister() {
	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCmds::Get().RefreshPipelines, FExecuteAction::CreateRaw(this, &SGdhVideoEncoderTool::OnRefreshPipelines)
	);
}

void SGdhVideoEncoderTool::OnRefreshPipelines() {

	// 1. for every sequence that specified in pipeline asset
	//	1.1. get level sequence asset
	//  1.2 create new empty mrq asset
	//  1.3 load sequence to that queue with specified settings
	//  1.4 generate encode command
	//  1.5 run ffmpeg encode command

	if (!VideoEncoderToolSettings->Pipeline.LoadSynchronous()) return;

	UMoviePipelineQueue* Queue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!Queue) return;

	for (const auto& Job : Queue->GetJobs()) { Queue->DeleteJob(Job); }

	UMoviePipelineMasterConfig* MasterConfig = NewObject<UMoviePipelineMasterConfig>();
	if (!MasterConfig) return;

	MasterConfig->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
	MasterConfig->FindOrAddSettingByClass(UMoviePipelineImageSequenceOutput_PNG::StaticClass());

	UMoviePipelineOutputSetting* OutputSetting = Cast<UMoviePipelineOutputSetting>(
		MasterConfig->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass())
	);
	if (!OutputSetting) return;

	OutputSetting->FileNameFormat = TEXT("{sequence_name}.{frame_number_rel}");
	OutputSetting->OutputResolution = FIntPoint(1920, 1080);
	OutputSetting->ZeroPadFrameNumbers = 4;
	OutputSetting->FrameNumberOffset = 0;
	OutputSetting->OutputFrameRate = FFrameRate(30, 1);

	// IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const auto& Pipeline = VideoEncoderToolSettings->Pipeline.Get();
	const FString OutDir = FString::Printf(
		TEXT("%s/%s"), *FPaths::ConvertRelativePathToFull(Pipeline->OutputDir.Path), *Pipeline->DirImages
	);
	OutputSetting->OutputDirectory.Path = OutDir;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (FPaths::DirectoryExists(OutDir)) { PlatformFile.DeleteDirectory(*OutDir); }
	PlatformFile.CreateDirectoryTree(*OutDir);

	Pipeline->EncodeCmdUpdate();
	const FString MainEncodeCmd = Pipeline->EncodeCmdVis;

	EncodeCmds.Reset();

	for (const auto& Sequence : Pipeline->Sequences) {
		const auto Job = Queue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
		Job->Map = Pipeline->Level.LoadSynchronous();
		Job->SetSequence(Sequence.LoadSynchronous());
		Job->SetConfiguration(MasterConfig);

		// ffmpeg -i {input} => ffmpeg -i LS_Test.%04d.png
		const FString SequenceName = Sequence->GetName();
		const FString Input = FString::Printf(TEXT("\"%s/%s.%%04d.png\""), *OutDir, *SequenceName);
		const FString Output = FString::Printf(TEXT("%s/%s"), *OutDir, *SequenceName);
		const FString EncodeCmd = MainEncodeCmd.Replace(TEXT("{input}"), *Input).Replace(TEXT("{output}"), *Output);

		EncodeCmds.Add(EncodeCmd);
	}

	const auto Executor =
		Cast<UMoviePipelinePIEExecutor>(GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()
											->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass()));
	if (!Executor) return;

	Executor->OnExecutorFinished().AddRaw(this, &SGdhVideoEncoderTool::OnRenderFinished);
}

void SGdhVideoEncoderTool::OnRenderFinished(UMoviePipelineExecutorBase*, bool bSuccess) {
	if (!bSuccess) return;

	uint32 ProcessId;
	const FString FFmpegPath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg.exe"));

	void* PipeRead = nullptr;
	void* PipeWrite = nullptr;

	for (const auto& Cmd : EncodeCmds) {
		verify(FPlatformProcess::CreatePipe(PipeRead, PipeWrite));

		FProcHandle ProcessHandle = FPlatformProcess::CreateProc(
			*FFmpegPath, *Cmd, true, false, false, &ProcessId, 0, nullptr, PipeRead	  // Redirect stderr
		);

		if (ProcessHandle.IsValid()) {
			FString ErrorOutput;
			while (FPlatformProcess::IsProcRunning(ProcessHandle)) {
				FString PartialOutput = FPlatformProcess::ReadPipe(PipeRead);
				if (!PartialOutput.IsEmpty()) { ErrorOutput += PartialOutput; }

				FPlatformProcess::Sleep(0.1f);	 // Prevent high CPU usage
			}

			// Read remaining data after process exits
			FString FinalOutput = FPlatformProcess::ReadPipe(PipeRead);
			if (!FinalOutput.IsEmpty()) { ErrorOutput += FinalOutput; }

			// Check exit code
			int32 ReturnCode = 0;
			if (FPlatformProcess::GetProcReturnCode(ProcessHandle, &ReturnCode)) {
				if (ReturnCode != 0)   // FFmpeg failed
				{
					FString ErrorMessage = FString::Printf(
						TEXT("FFmpeg failed with exit code: %d\nError Output:\n%s"), ReturnCode, *ErrorOutput
					);

					UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
					FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
				}
			}
			else { UE_LOG(LogTemp, Error, TEXT("Failed to retrieve FFmpeg process return code.")); }

			// Cleanup
			FPlatformProcess::CloseProc(ProcessHandle);
			FPlatformProcess::ClosePipe(PipeRead, PipeWrite);
		}
		else {
			FString ErrorMessage = TEXT("Failed to start FFmpeg process.");
			UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
		}
	}
}

TSharedRef<SWidget> SGdhVideoEncoderTool::CreateToolbarMain() const {

	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};

	ToolBarBuilder.BeginSection("GdhVideoEncoderToolMainToolbar");
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().RefreshPipelines);
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}
