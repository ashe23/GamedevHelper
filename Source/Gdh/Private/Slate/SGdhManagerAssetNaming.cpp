// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhManagerAssetNaming.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "GdhSubsystem.h"
#include "GdhTypes.h"
#include "Settings/GdhAssetScanSettings.h"
#include "Slate/SGdhManagerAssetNamingItem.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

void SGdhManagerAssetNaming::Construct(const FArguments& InArgs)
{
	ScanSettings = GetMutableDefault<UGdhAssetScanSettings>();

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
				// todo:ashe23 if anything selected rename only selected, if not then all assets
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
							SettingsProperty.ToSharedRef()
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
						.HeaderRow(GetHeaderRow())
					]
				]
			]
		]
	];
}

void SGdhManagerAssetNaming::UpdateListData()
{
	TArray<FAssetData> AssetsAll;
	UGdhSubsystem::GetAssetsAll(AssetsAll);

	ListItems.Reset(AssetsAll.Num());

	for (const auto& Asset : AssetsAll)
	{
		UGdhManagerAssetNamingItem* NewItem = NewObject<UGdhManagerAssetNamingItem>();
		if (!NewItem) continue;

		NewItem->AssetData = Asset;
		NewItem->OldName = Asset.AssetName.ToString();
		NewItem->NewName = Asset.AssetName.ToString(); // todo:ashe23 make rename preview and get new name and note if there is one
		NewItem->Note = TEXT("Some Note");

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
