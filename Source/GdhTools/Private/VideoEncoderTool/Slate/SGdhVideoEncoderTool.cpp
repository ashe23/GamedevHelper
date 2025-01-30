// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "VideoEncoderTool/Slate/SGdhVideoEncoderTool.h"
#include "VideoEncoderTool/GdhVideoEncoderToolSettings.h"
#include "VideoEncoderTool/Slate/SGdhVideoEncoderToolListItem.h"
#include "GdhCmds.h"
#include "GdhLibEditor.h"
#include "GdhLibPath.h"
#include "GdhStyles.h"
#include "GdhToolsModule.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "MoviePipelineConfigBase.h"
#include "MoviePipelineDeferredPasses.h"
#include "MoviePipelineImageSequenceOutput.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelinePIEExecutor.h"
#include "MoviePipelineQueueSubsystem.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void SGdhVideoEncoderTool::Construct(const FArguments& InArgs) {

	VideoEncoderToolSettings = GetMutableDefault<UGdhVideoEncoderToolSettings>();
	if (!VideoEncoderToolSettings.IsValid()) return;

	CmdsRegister();
	ListUpdateData();
	ListUpdateView();

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
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhVideoEncoderToolListItem>>)
						.ListItemsSource(&ListItems)
						// .ClearSelectionOnClick(true)
						.SelectionMode(ESelectionMode::None)
						.OnGenerateRow(this, &SGdhVideoEncoderTool::OnGenerateRow)
						.HeaderRow(GetHeaderRow())
					]
				]
			]
		]
	];
	// clang-format on
}

void SGdhVideoEncoderTool::ListUpdateData() {
	if (!VideoEncoderToolSettings.IsValid()) return;
	if (!VideoEncoderToolSettings->RenderQueue.LoadSynchronous()) return;
	if (!VideoEncoderToolSettings->RenderSettings.LoadSynchronous()) return;

	const UMoviePipelineOutputSetting* OutputSetting = VideoEncoderToolSettings->RenderSettings->FindSetting<UMoviePipelineOutputSetting>(false);
	if (!OutputSetting) return;

	const FString DirOutput = FPaths::ConvertRelativePathToFull(OutputSetting->OutputDirectory.Path);
	const FString DirImages = FString::Printf(TEXT("%s/images"), *DirOutput);

	const auto Jobs = VideoEncoderToolSettings->RenderQueue->GetJobs();

	ListItems.Reset(Jobs.Num());
	EncodeCmds.Reset(Jobs.Num());

	for (const auto& Job : Jobs) {
		if (!Job) continue;

		UGdhVideoEncoderToolListItem* NewItem = NewObject<UGdhVideoEncoderToolListItem>();
		if (!NewItem) continue;

		NewItem->NameQueue = VideoEncoderToolSettings->RenderQueue->GetName();
		NewItem->NameSequence = Job->Sequence.GetAssetName();

		// TODO:ashe23 update this token list later
		// {ffmpeg} - path to ffmpeg executable on the system. Query from system env or give user option to specify?
		// {input} - image sequence input format for ffmpeg with full path. Example: "D:/Renders/Test.%04d.png".
		// {seq_name} - name of level sequence. Example: "Test"
		// {seq_path} - full path to sequence. Example: "D:/Renders/Test"

		// TODO:ashe23 also need to think about audio mixing options and multiple encoding options that must run continuously

		const FString EncodeCmd = UKismetStringLibrary::JoinStringArray(VideoEncoderToolSettings->EncodeCmd, TEXT(" "));
		const FString TokenFFmpegPath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg.exe"));
		const FString TokenInput = FString::Printf(TEXT("%s/%s.%%04d.png"), *DirOutput, *NewItem->NameSequence);
		const FString TokenSeqName = NewItem->NameSequence;
		const FString TokenSeqPath = FString::Printf(TEXT("%s/%s"), *DirOutput, *NewItem->NameSequence);
		const FString EncodeCmdPreview = EncodeCmd.Replace(TEXT("{ffmpeg}"), *TokenFFmpegPath)
											 .Replace(TEXT("{input}"), *TokenInput)
											 .Replace(TEXT("{seq_path}"), *TokenSeqPath)
											 .Replace(TEXT("{seq_name}"), *TokenSeqName);
		const FString EncodeCmdInternal = EncodeCmd.Replace(TEXT("{ffmpeg}"), TEXT(" "))
											  .Replace(TEXT("{input}"), *TokenInput)
											  .Replace(TEXT("{seq_path}"), *TokenSeqPath)
											  .Replace(TEXT("{seq_name}"), *TokenSeqName);

		NewItem->EncodeCmdPreview = EncodeCmdPreview;

		ListItems.Add(NewItem);
		// here we need encode command without {ffmpeg} part, because CreateProc function requires to specify ffmpeg path separately
		EncodeCmds.Add(EncodeCmdInternal);
	}
}

void SGdhVideoEncoderTool::ListUpdateView() {
	if (!ListView) return;

	ListView->RebuildList();
}

void SGdhVideoEncoderTool::CmdsRegister() {
	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(FGdhCmds::Get().RefreshPipelines, FExecuteAction::CreateRaw(this, &SGdhVideoEncoderTool::OnRefreshPipelines));
	Cmds->MapAction(FGdhCmds::Get().Process, FExecuteAction::CreateRaw(this, &SGdhVideoEncoderTool::OnProcess));
}

void SGdhVideoEncoderTool::OnRefreshPipelines() {

	ListUpdateData();
	ListUpdateView();
}

void SGdhVideoEncoderTool::OnProcess() {
	// const FString FFmpegPath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg.exe"));
	//
	// if (FFmpegPath.IsEmpty()) {
	// 	FMessageDialog::Open(
	// 		EAppMsgType::Ok,
	// 		FText::FromString(TEXT("FFmpeg path not found! Make sure ffmpeg executable is available in sys ENV paths"))
	// 	);
	// 	return;
	// }

	// requirments for rendering
	// 1. level sequence has valid camera

	// 1. for every sequence that specified in pipeline asset
	//	1.1. get level sequence asset
	//  1.2 create new empty mrq asset
	//  1.3 load sequence to that queue with specified settings
	//  1.4 generate encode command
	//  1.5 run ffmpeg encode command

	// if (!VideoEncoderToolSettings->Pipeline.LoadSynchronous()) return;
	//
	UMoviePipelineQueue* Queue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!Queue) return;

	Queue->CopyFrom(VideoEncoderToolSettings->RenderQueue.LoadSynchronous());

	for (const auto& Job : Queue->GetJobs()) {
		Job->SetConfiguration(VideoEncoderToolSettings->RenderSettings.LoadSynchronous());
	}

	//
	// for (const auto& Job : Queue->GetJobs()) {
	// 	Queue->DeleteJob(Job);
	// }

	// UMoviePipelineMasterConfig* MasterConfig = NewObject<UMoviePipelineMasterConfig>();
	// if (!MasterConfig) return;
	//
	// MasterConfig->FindOrAddSettingByClass(UMoviePipelineDeferredPassBase::StaticClass());
	// MasterConfig->FindOrAddSettingByClass(UMoviePipelineImageSequenceOutput_PNG::StaticClass());
	//
	// UMoviePipelineOutputSetting* OutputSetting = Cast<UMoviePipelineOutputSetting>(
	// 	MasterConfig->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass())
	// );
	// if (!OutputSetting) return;
	//
	// OutputSetting->FileNameFormat = TEXT("{sequence_name}.{frame_number_rel}");
	// OutputSetting->OutputResolution = FIntPoint(1920, 1080);
	// OutputSetting->ZeroPadFrameNumbers = 4;
	// OutputSetting->FrameNumberOffset = 0;
	// OutputSetting->OutputFrameRate = FFrameRate(30, 1);
	//
	// // IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// const auto& Pipeline = VideoEncoderToolSettings->Pipeline.Get();
	// const FString OutDir = FString::Printf(
	// 	TEXT("%s/%s"), *FPaths::ConvertRelativePathToFull(Pipeline->OutputDir.Path), *Pipeline->DirImages
	// );
	// OutputSetting->OutputDirectory.Path = OutDir;
	//
	// IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// if (FPaths::DirectoryExists(OutDir)) { PlatformFile.DeleteDirectory(*OutDir); }
	// PlatformFile.CreateDirectoryTree(*OutDir);
	//
	// Pipeline->EncodeCmdUpdate();
	// const FString MainEncodeCmd = Pipeline->EncodeCmdVis;

	// EncodeCmds.Reset();

	// for (const auto& Job : VideoEncoderToolSettings->RenderQueue->GetJobs()) {
	// 	const auto NewJob = Queue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());
	// 	NewJob->Map = Job->.LoadSynchronous();
	// 	NewJob->SetSequence(Sequence.LoadSynchronous());
	// 	NewJob->SetConfiguration(MasterConfig);
	//
	// 	// ffmpeg -i {input} => ffmpeg -i LS_Test.%04d.png
	// 	const FString SequenceName = Sequence->GetName();
	// 	const FString Input = FString::Printf(TEXT("\"%s/%s.%%04d.png\""), *OutDir, *SequenceName);
	// 	const FString Output = FString::Printf(TEXT("%s/%s"), *OutDir, *SequenceName);
	// 	const FString EncodeCmd = MainEncodeCmd.Replace(TEXT("{input}"), *Input).Replace(TEXT("{output}"), *Output);
	//
	// 	EncodeCmds.Add(EncodeCmd);
	// }

	// GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass());

	const auto Executor = Cast<UMoviePipelinePIEExecutor>(
		GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(UMoviePipelinePIEExecutor::StaticClass())
	);
	if (!Executor) return;

	Executor->OnExecutorFinished().AddRaw(this, &SGdhVideoEncoderTool::OnRenderFinished);
}

void SGdhVideoEncoderTool::OnRenderFinished(UMoviePipelineExecutorBase*, bool bSuccess) {
	if (!bSuccess) return;

	// TODO:ashe23 ideally this path should be cached somewhere and checked before proceeding here
	const FString FFmpegPath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg.exe"));

	for (const auto& Cmd : EncodeCmds) {

		uint32 ProcessId;
		FProcHandle Handle = FPlatformProcess::CreateProc(*FFmpegPath, *Cmd, true, false, false, &ProcessId, 0, nullptr, nullptr);
		if (!Handle.IsValid()) {
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to run ffmpeg command!")));
			return;
		}

		while (FPlatformProcess::IsProcRunning(Handle)) {
			FPlatformProcess::Sleep(0.1f);
		}

		int32 ReturnCode = 0;
		if (FPlatformProcess::GetProcReturnCode(Handle, &ReturnCode) && ReturnCode != 0) {
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("FFmpeg command failed to finish!")));
			FPlatformProcess::CloseProc(Handle);
			return;
		}

		FPlatformProcess::CloseProc(Handle);
	}

	// void* PipeRead = nullptr;
	// void* PipeWrite = nullptr;
	//
	// for (const auto& Cmd : EncodeCmds) {
	// 	verify(FPlatformProcess::CreatePipe(PipeRead, PipeWrite));
	//
	// 	FProcHandle ProcessHandle = FPlatformProcess::CreateProc(
	// 		*FFmpegPath, *Cmd, true, false, false, &ProcessId, 0, nullptr, PipeRead	  // Redirect stderr
	// 	);
	//
	// 	if (ProcessHandle.IsValid()) {
	// 		FString ErrorOutput;
	// 		while (FPlatformProcess::IsProcRunning(ProcessHandle)) {
	// 			FString PartialOutput = FPlatformProcess::ReadPipe(PipeRead);
	// 			if (!PartialOutput.IsEmpty()) {
	// 				ErrorOutput += PartialOutput;
	// 			}
	//
	// 			FPlatformProcess::Sleep(0.1f);	 // Prevent high CPU usage
	// 		}
	//
	// 		// Read remaining data after process exits
	// 		FString FinalOutput = FPlatformProcess::ReadPipe(PipeRead);
	// 		if (!FinalOutput.IsEmpty()) {
	// 			ErrorOutput += FinalOutput;
	// 		}
	//
	// 		// Check exit code
	// 		int32 ReturnCode = 0;
	// 		if (FPlatformProcess::GetProcReturnCode(ProcessHandle, &ReturnCode)) {
	// 			if (ReturnCode != 0)   // FFmpeg failed
	// 			{
	// 				FString ErrorMessage = FString::Printf(TEXT("FFmpeg failed with exit code: %d\nError Output:\n%s"), ReturnCode, *ErrorOutput);
	//
	// 				UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	// 				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
	// 			}
	// 		}
	// 		else {
	// 			UE_LOG(LogTemp, Error, TEXT("Failed to retrieve FFmpeg process return code."));
	// 		}
	//
	// 		// Cleanup
	// 		FPlatformProcess::CloseProc(ProcessHandle);
	// 		FPlatformProcess::ClosePipe(PipeRead, PipeWrite);
	// 	}
	// 	else {
	// 		FString ErrorMessage = TEXT("Failed to start FFmpeg process.");
	// 		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	// 		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
	// 	}
	// }
}

TSharedRef<SWidget> SGdhVideoEncoderTool::CreateToolbarMain() const {

	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};

	ToolBarBuilder.BeginSection("GdhVideoEncoderToolMainToolbar");
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().RefreshPipelines);
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().Process);
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SHeaderRow> SGdhVideoEncoderTool::GetHeaderRow() {
	// clang-format off

	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(TEXT("NameQueue"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("NameQueue")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("NameSequence"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("NameSequence")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Preview"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Preview")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];

	// clang-format on
}

TSharedRef<ITableRow>
SGdhVideoEncoderTool::OnGenerateRow(TWeakObjectPtr<UGdhVideoEncoderToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGdhVideoEncoderToolListItem, OwnerTable).ListItem(Item);
}
