// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "VideoEncoderTool/Slate/SGdhVideoEncoderTool.h"
#include "VideoEncoderTool/GdhVideoEncoderToolSettings.h"
#include "VideoEncoderTool/Slate/SGdhVideoEncoderToolListItem.h"
#include "GdhCmds.h"
#include "GdhLibAsset.h"
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
#include "Misc/ScopedSlowTask.h"

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

	ListItems.Reset();

	for (const auto& Queue : VideoEncoderToolSettings->RenderQueues) {

		if (!Queue.LoadSynchronous()) continue;

		ListItems.Reserve(ListItems.Num() + Queue->GetJobs().Num());

		for (const auto& Job : Queue->GetJobs()) {
			if (!Job) continue;

			UGdhVideoEncoderToolListItem* NewItem = NewObject<UGdhVideoEncoderToolListItem>();
			if (!NewItem) continue;

			const ULevelSequence* Sequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			if (!Sequence) continue;

			const FFrameRate FrameRate = UGdhLibAsset::GetLevelSequenceFrameRate(Sequence);
			const int32 FrameStart = UGdhLibAsset::GetLevelSequenceStartFrame(Sequence, FrameRate);
			const int32 FrameEnd = UGdhLibAsset::GetLevelSequenceEndFrame(Sequence, FrameRate);
			const int32 DurationInFrames = UGdhLibAsset::GetLevelSequenceDurationInFrames(Sequence, FrameRate);
			const float DurationInSec = UGdhLibAsset::GetLevelSequenceDurationInSeconds(Sequence, FrameRate);

			NewItem->NameQueue = Queue->GetName();
			NewItem->NameSequence = Job->Sequence.GetAssetName();
			NewItem->FrameStart = FString::FromInt(FrameStart);
			NewItem->FrameEnd = FString::FromInt(FrameEnd);
			NewItem->FrameRate = FrameRate.ToPrettyText().ToString();
			NewItem->Duration = FString::Printf(TEXT("%d frames(%.2f sec)"), DurationInFrames, DurationInSec);

			ListItems.Add(NewItem);
		}
	}
}

void SGdhVideoEncoderTool::ListUpdateView() const {
	if (!ListView) return;

	ListView->RebuildList();
}

void SGdhVideoEncoderTool::CmdsRegister() {
	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(FGdhCmds::Get().VetRefresh, FExecuteAction::CreateRaw(this, &SGdhVideoEncoderTool::OnVetRefresh));
	Cmds->MapAction(FGdhCmds::Get().VetProcess, FExecuteAction::CreateRaw(this, &SGdhVideoEncoderTool::OnVetProcess));
}

void SGdhVideoEncoderTool::OnVetRefresh() {
	ValidateSettings();
	ListUpdateData();
	ListUpdateView();
}

void SGdhVideoEncoderTool::OnVetProcess() {
	ValidateSettings();

	UMoviePipelineQueue* MasterQueue = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->GetQueue();
	if (!MasterQueue) return;

	// removing all jobs from queue
	for (const auto& Job : MasterQueue->GetJobs()) {
		MasterQueue->DeleteJob(Job);
	}

	// creating main render config class
	UMoviePipelineMasterConfig* MasterConfig = VideoEncoderToolSettings->RenderSettings.Get();
	if (!MasterConfig) return;

	UMoviePipelineOutputSetting* OutputSetting =
		Cast<UMoviePipelineOutputSetting>(MasterConfig->FindOrAddSettingByClass(UMoviePipelineOutputSetting::StaticClass()));
	if (!OutputSetting) return;

	const FString DirOutput = FPaths::ConvertRelativePathToFull(VideoEncoderToolSettings->DirOutput.Path);
	const FString DirNameImagesRaw = VideoEncoderToolSettings->DirNameImages.IsEmpty() ? TEXT("images") : VideoEncoderToolSettings->DirNameImages;
	const FString DirNameVideosRaw = VideoEncoderToolSettings->DirNameVideo.IsEmpty() ? TEXT("videos") : VideoEncoderToolSettings->DirNameVideo;
	const FString DirNameImages = FPaths::MakeValidFileName(DirNameImagesRaw);
	const FString DirNameVideos = FPaths::MakeValidFileName(DirNameVideosRaw);
	const FString DirImages = FString::Printf(TEXT("%s/%s"), *DirOutput, *DirNameImages);
	const FString DirVideos = FString::Printf(TEXT("%s/%s"), *DirOutput, *DirNameVideos);

	OutputSetting->FileNameFormat = TEXT("{sequence_name}.{frame_number_rel}");
	OutputSetting->ZeroPadFrameNumbers = 4;
	OutputSetting->FrameNumberOffset = 0;
	OutputSetting->bUseCustomPlaybackRange = true;

	EncodeCmds.Reset();

	for (const auto& Queue : VideoEncoderToolSettings->RenderQueues) {
		for (const auto& Job : Queue->GetJobs()) {
			const auto NewJob = MasterQueue->AllocateNewJob(UMoviePipelineExecutorJob::StaticClass());

			const ULevelSequence* Sequence = Cast<ULevelSequence>(Job->Sequence.TryLoad());
			if (!Sequence) continue;

			NewJob->Map = Job->Map;
			NewJob->SetSequence(Sequence);

			// {dir_output}/{dir_images_name}/{queue_name}/{sequence_name}/
			const FString NameQueue = Queue->GetName();
			const FString NameSequence = Sequence->GetName();
			const FString JobOutputDirImages = FString::Printf(TEXT("%s/%s/%s"), *DirImages, *NameQueue, *NameSequence);

			if (!FPaths::DirectoryExists(*JobOutputDirImages)) {
				FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*JobOutputDirImages);
			}

			OutputSetting->OutputDirectory.Path = JobOutputDirImages;
			OutputSetting->CustomStartFrame = UGdhLibAsset::GetLevelSequenceStartFrame(Sequence, OutputSetting->OutputFrameRate);
			OutputSetting->CustomEndFrame = UGdhLibAsset::GetLevelSequenceEndFrame(Sequence, OutputSetting->OutputFrameRate);

			NewJob->SetConfiguration(MasterConfig);

			for (const auto& Pipeline : VideoEncoderToolSettings->EncodePipeline) {
				const FString PipeName = Pipeline.Key;
				const FString PipeCmd = Pipeline.Value;

				const FString JobOutputDirVideos = FString::Printf(TEXT("%s/%s/%s"), *DirVideos, *PipeName, *NameQueue);

				if (!FPaths::DirectoryExists(*JobOutputDirVideos)) {
					FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*JobOutputDirVideos);
				}

				const FString TokenFFmpeg = FFmpegPath;
				// TODO:ashe23 query image extension from render settings classes
				const FString TokenInput = FString::Printf(TEXT("%s/%s.%%04d.png"), *JobOutputDirImages, *NameSequence);
				const FString TokenOutput = FString::Printf(TEXT("%s/%s"), *JobOutputDirVideos, *NameSequence);
				FString InternalCmd = PipeCmd;
				InternalCmd.RemoveFromStart(TEXT("{ffmpeg}"));
				InternalCmd = InternalCmd.Replace(TEXT("{input}"), *TokenInput);
				InternalCmd = InternalCmd.Replace(TEXT("{output}"), *TokenOutput);

				EncodeCmds.Add(InternalCmd);
			}
		}
	}

	if (GEditor) {
		const auto ExecutorClass = VideoEncoderToolSettings->ExecutorClass.LoadSynchronous();
		if (!ExecutorClass) return;

		const auto Executor = GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->RenderQueueWithExecutor(ExecutorClass);
		Executor->OnExecutorFinished().AddRaw(this, &SGdhVideoEncoderTool::OnRenderFinished);
	}
}

void SGdhVideoEncoderTool::OnRenderFinished(UMoviePipelineExecutorBase*, bool bSuccess) {
	if (!bSuccess) return;
	if (FFmpegPath.IsEmpty()) return;

	FScopedSlowTask SlowTask {static_cast<float>(EncodeCmds.Num()), FText::FromString(TEXT("Encoding..."))};
	SlowTask.MakeDialog(false, false);

	for (const auto& Cmd : EncodeCmds) {
		SlowTask.EnterProgressFrame(1.0f, FText::FromString(Cmd));

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

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Encoding Done.")));
}

void SGdhVideoEncoderTool::ValidateSettings() {
	if (!VideoEncoderToolSettings.IsValid()) return;

	// TODO:ashe23 for now we will show error via message dialogs and when process of refresh buttons clicked. Later on change it and integrate to UI

	// checking ffmpeg path
	FFmpegPath = UGdhLibPath::GetPathFromEnv(TEXT("ffmpeg.exe"));	// TODO:ashe23 on linux this might differ
	if (FFmpegPath.IsEmpty()) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("FFmpeg Path Not Found. Make sure its available in system ENV paths")));
		return;
	}

	// checking if output directory exists
	// IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString DirOutput = VideoEncoderToolSettings->DirOutput.Path;
	if (DirOutput.IsEmpty() || !FPaths::DirectoryExists(FPaths::ConvertRelativePathToFull(VideoEncoderToolSettings->DirOutput.Path))) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Output Directory Not Specified!")));
		return;
	}

	// checking if render settings are specified
	if (!VideoEncoderToolSettings->RenderSettings.LoadSynchronous()) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Render Settings Not Specified!")));
		return;
	}

	// checking if queue is empty
	if (VideoEncoderToolSettings->RenderQueues.Num() == 0) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Render Queue is empty. Select some assets to render.")));
		return;
	}

	// render settings must have (deferred or path trace settings specified)
	// render settings must have (image format specified)
	// render resolution must be power of 2
}

TSharedRef<SWidget> SGdhVideoEncoderTool::CreateToolbarMain() const {

	FToolBarBuilder ToolBarBuilder {Cmds, FMultiBoxCustomization::None};

	ToolBarBuilder.BeginSection("GdhVideoEncoderToolMainToolbar");
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().VetRefresh);
	ToolBarBuilder.AddSeparator();
	ToolBarBuilder.AddToolBarButton(FGdhCmds::Get().VetProcess);
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
			.Text(FText::FromString(TEXT("Queue")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("NameSequence"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Sequence")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("FrameStart"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("FrameStart")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("FrameEnd"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("FrameEnd")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("FrameRate"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("FrameRate")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Duration"))
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Duration")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];

	// clang-format on
}

TSharedRef<ITableRow>
SGdhVideoEncoderTool::OnGenerateRow(TWeakObjectPtr<UGdhVideoEncoderToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGdhVideoEncoderToolListItem, OwnerTable).ListItem(Item);
}
