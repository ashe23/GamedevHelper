// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhManagerAssetNaming.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "GdhSubsystem.h"
#include "GdhTypes.h"
#include "Misc/ScopedSlowTask.h"
#include "Settings/GdhAssetScanSettings.h"
#include "Settings/GdhAssetNamingConvention.h"
#include "Slate/SGdhManagerAssetNamingItem.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhManagerAssetNaming::Construct(const FArguments& InArgs)
{
	ScanSettings = GetMutableDefault<UGdhAssetScanSettings>();
	AssetNamingConvention = GetMutableDefault<UGdhAssetNamingConvention>();

	if (!ScanSettings.IsValid() || !AssetNamingConvention.IsValid()) return;

	ScanSettings->OnSettingsChanged().AddRaw(this, &SGdhManagerAssetNaming::OnSettingsChanged);

	FDetailsViewArgs ViewArgs;
	ViewArgs.bUpdatesFromSelection = false;
	ViewArgs.bLockable = false;
	ViewArgs.bShowScrollBar = true;
	ViewArgs.bAllowSearch = false;
	ViewArgs.bShowOptions = false;
	ViewArgs.bAllowFavoriteSystem = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.ViewIdentifier = "GdhAssetScanSetting";

	const TSharedPtr<IDetailsView> SettingsProperty = UGdhSubsystem::GetModulePropertyEditor().CreateDetailView(ViewArgs);
	SettingsProperty->SetObject(ScanSettings.Get());

	ViewArgs.ViewIdentifier = "GdhAssetNamingConvention";
	const TSharedPtr<IDetailsView> AssetNamingConventionProperty = UGdhSubsystem::GetModulePropertyEditor().CreateDetailView(ViewArgs);
	AssetNamingConventionProperty->SetObject(AssetNamingConvention.Get());

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(
		FGdhCommands::Get().Cmd_ScanAssets,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				UpdateListData();
				UpdateListView();
			})
		)
	);
	Cmds->MapAction(
		FGdhCommands::Get().Cmd_RenameAssets,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				if (ListItems.Num() == 0)
				{
					UGdhSubsystem::ShowNotification(TEXT("No Asset to Rename"), SNotificationItem::CS_Fail, 5.0f);
					return;
				}

				const FText Title = FText::FromString(TEXT("Rename Assets"));
				const FText Context = FText::FromString(TEXT("Are you sure you want to rename selected assets?"));

				const EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::YesNo, Context, &Title);
				if (ReturnType == EAppReturnType::Cancel || ReturnType == EAppReturnType::No) return;

				FScopedSlowTask SlowTask(
					ListItems.Num(),
					FText::FromString("Renaming assets")
				);
				SlowTask.MakeDialog(true);

				TArray<FAssetRenameData> RenameDatas;
				RenameDatas.Reserve(ListItems.Num());

				TArray<TWeakObjectPtr<UGdhManagerAssetNamingItem>> SelectedItems = ListView.IsValid() && ListView->GetSelectedItems().Num() > 0 ? ListView->GetSelectedItems() : ListItems;

				for (const auto& Item : SelectedItems)
				{
					SlowTask.EnterProgressFrame(1.0f);

					if (!Item.IsValid()) continue;

					RenameDatas.Emplace(
						FAssetRenameData{
							Item->AssetData.GetAsset(),
							Item->AssetData.PackagePath.ToString(),
							Item->NewName
						});
				}

				if (UGdhSubsystem::GetModuleAssetTools().Get().RenameAssets(RenameDatas))
				{
					UGdhSubsystem::ShowNotification(FString::Printf(TEXT("Renamed %d of %d assets"), ListItems.Num(), ListItems.Num()), SNotificationItem::CS_Success, 5.0f);

					TArray<FAssetData> Redirectors;
					UGdhSubsystem::GetProjectRedirectors(Redirectors);

					UGdhSubsystem::FixProjectRedirectors(Redirectors, true);
				}
				else
				{
					UGdhSubsystem::ShowNotificationWithOutputLog(TEXT("Failed to rename some assets"), SNotificationItem::CS_Fail, 5.0f);
				}

				UpdateListData();
				UpdateListView();
			})
		)
	);
	Cmds->MapAction(
		FGdhCommands::Get().Cmd_ClearSelection,
		FUIAction(
			FExecuteAction::CreateLambda([&]()
			{
				UpdateListView();
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				return ListView.IsValid() && ListView->GetSelectedItems().Num() > 0;
			})
		)
	);

	UpdateListData();
	UpdateListView();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
		[
			SNew(SSplitter)
			.PhysicalSplitterHandleSize(3.0f)
			.Style(FEditorStyle::Get(), "DetailsView.Splitter")
			+ SSplitter::Slot().Value(0.3f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight().Padding(3.0f)
				[
					SNew(SBox)
					[
						SNew(SScrollBox)
						.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
						.AnimateWheelScrolling(true)
						.AllowOverscroll(EAllowOverscroll::No)
						+ SScrollBox::Slot()
						[
							SettingsProperty.ToSharedRef()
						]
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(3.0f)
				[
					SNew(SSeparator).Thickness(5.0f)
				]
				+ SVerticalBox::Slot().FillHeight(1.0f).Padding(3.0f)
				[
					SNew(SBox)
					[
						SNew(SScrollBox)
						.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
						.AnimateWheelScrolling(true)
						.AllowOverscroll(EAllowOverscroll::No)
						+ SScrollBox::Slot()
						[
							AssetNamingConventionProperty.ToSharedRef()
						]
					]
				]
			]
			+ SSplitter::Slot().Value(0.7f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight().Padding(3.0f)
				[
					CreateToolbarMain()
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(3.0f)
				[
					SNew(SSeparator).Thickness(5.0f)
				]
				+ SVerticalBox::Slot().FillHeight(1.0f).Padding(3.0f)
				[
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhManagerAssetNamingItem>>)
						.ListItemsSource(&ListItems)
						.SelectionMode(ESelectionMode::Multi)
						.OnGenerateRow(this, &SGdhManagerAssetNaming::OnGenerateRow)
						.OnMouseButtonDoubleClick(this, &SGdhManagerAssetNaming::OnListRowMouseDoubleClick)
						.HeaderRow(GetHeaderRow())
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(3.0f, 0.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock).Text_Raw(this, &SGdhManagerAssetNaming::GetListSummaryTxt)
					]
				]
			]
		]
	];
}

void SGdhManagerAssetNaming::UpdateListData()
{
	if (UGdhSubsystem::GetModuleAssetRegistry().GetRegistry().IsLoadingAssets()) return;

	TArray<FAssetData> Redirectors;
	UGdhSubsystem::GetProjectRedirectors(Redirectors);

	if (Redirectors.Num() > 0)
	{
		UGdhSubsystem::FixProjectRedirectors(Redirectors, true);
	}

	if (UGdhSubsystem::ProjectHasRedirectors())
	{
		UGdhSubsystem::ShowNotificationWithOutputLog(TEXT("Project contains redirectors that cant be fixed automatically. Please fix them manually and try again"), SNotificationItem::CS_Fail, 5.0f);
		return;
	}

	TArray<FAssetData> AssetsAll;

	if (ScanSettings->ScanPath.Path.IsEmpty())
	{
		UGdhSubsystem::GetAssetsAll(AssetsAll);
	}
	else
	{
		UGdhSubsystem::GetAssetsByPath(ScanSettings->ScanPath.Path, ScanSettings->bScanRecursive, AssetsAll);
	}

	ListItems.Reset(AssetsAll.Num());

	TMap<FName, TSet<FString>> RenamePreviews;
	RenamePreviews.Reserve(AssetsAll.Num());

	for (const auto& Asset : AssetsAll)
	{
		UGdhManagerAssetNamingItem* NewItem = NewObject<UGdhManagerAssetNamingItem>();
		if (!NewItem) continue;

		const FString AssetNameOriginal = Asset.AssetName.ToString();
		const FString AssetNamePreview = UGdhSubsystem::GetAssetRenamePreview(Asset);
		if (AssetNamePreview.IsEmpty()) continue;

		const FString AssetPreviewObjectPath = Asset.PackagePath.ToString() + FString::Printf(TEXT("/%s.%s"), *AssetNamePreview, *AssetNamePreview);
		const FAssetData AssetData = UGdhSubsystem::GetAssetByObjectPath(AssetPreviewObjectPath);
		TSet<FString>& AssetsInPath = RenamePreviews.FindOrAdd(Asset.PackagePath);

		NewItem->AssetData = Asset;
		NewItem->OldName = AssetNameOriginal;
		NewItem->NewName = AssetNamePreview;

		if (AssetNameOriginal.Equals(AssetNamePreview, ESearchCase::CaseSensitive))
		{
			AssetsInPath.Add(AssetNamePreview);
			continue;
		}

		if (AssetData.IsValid() && AssetNameOriginal.Equals(AssetNamePreview, ESearchCase::CaseSensitive))
		{
			NewItem->Note = TEXT("Asset with same name already exists at this path");
			NewItem->NoteColor = FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red")).GetSpecifiedColor();
		}

		if (AssetsInPath.Contains(AssetNamePreview))
		{
			NewItem->Note = TEXT("Asset with same name already exists in previews");
			NewItem->NoteColor = FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red")).GetSpecifiedColor();
		}

		AssetsInPath.Add(AssetNamePreview);

		// if old name and new name have same name => OK 
		// if other asset with exactly same name exists in content browser => DuplicateNameContentBrowser
		// if other preview with exactly same and path exists => DuplicateNamePreview
		// else => OkToRename

		ListItems.Emplace(NewItem);
	}
}

void SGdhManagerAssetNaming::UpdateListView() const
{
	if (!ListView.IsValid()) return;

	ListView->ClearHighlightedItems();
	ListView->ClearSelection();
	ListView->RequestListRefresh();
}

void SGdhManagerAssetNaming::OnSettingsChanged()
{
	UpdateListData();
	UpdateListView();
}

TSharedRef<SWidget> SGdhManagerAssetNaming::CreateToolbarMain() const
{
	FToolBarBuilder ToolBarBuilder{Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhSectionMain");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().Cmd_ScanAssets);
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().Cmd_RenameAssets);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().Cmd_ClearSelection);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SHeaderRow> SGdhManagerAssetNaming::GetHeaderRow()
{
	return
		SNew(SHeaderRow)
		+ SHeaderRow::Column(TEXT("Preview"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .VAlignCell(VAlign_Center)
		  .HAlignCell(HAlign_Center)
		  .HeaderContentPadding(FMargin{10.0f})
		  .FillWidth(0.1f)
		  .FixedWidth(50.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("#")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("AssetClass"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.4f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .FixedWidth(200.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Type")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Result"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.3f)
		  .HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Preview")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Path"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.3f)
		  .HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Path")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		]
		+ SHeaderRow::Column(TEXT("Note"))
		  .HAlignHeader(HAlign_Center)
		  .VAlignHeader(VAlign_Center)
		  .FillWidth(0.3f)
		  .HeaderContentPadding(FMargin{5.0f})
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Note")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];
}

TSharedRef<ITableRow> SGdhManagerAssetNaming::OnGenerateRow(TWeakObjectPtr<UGdhManagerAssetNamingItem> Item, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGdhManagerAssetNamingItem, OwnerTable).RowItem(Item);
}

void SGdhManagerAssetNaming::OnListRowMouseDoubleClick(TWeakObjectPtr<UGdhManagerAssetNamingItem> Item)
{
	if (!Item.IsValid()) return;

	UGdhSubsystem::OpenAssetEditor(Item->AssetData);
}

FText SGdhManagerAssetNaming::GetListSummaryTxt() const
{
	if (ListView.IsValid())
	{
		const auto& SelectedItems = ListView->GetSelectedItems();
		if (SelectedItems.Num() > 0)
		{
			return FText::FromString(FString::Printf(TEXT("Total %d assets (Selected %d assets)"), ListItems.Num(), SelectedItems.Num()));
		}
	}
	return FText::FromString(FString::Printf(TEXT("Total %d assets"), ListItems.Num()));
}
