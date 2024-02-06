// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhAssetNamingTool.h"
#include "GdhCmds.h"
#include "GdhStyles.h"
#include "GdhSubsystem.h"
#include "Misc/ScopedSlowTask.h"
#include "Settings/GdhAssetNamingToolSettings.h"
#include "Slate/SGdhAssetNamingToolListItem.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SGdhAssetNamingTool::Construct(const FArguments& InArgs)
{
	AssetNamingToolSettings = GetMutableDefault<UGdhAssetNamingToolSettings>();
	if (!AssetNamingToolSettings.IsValid()) return;

	Cmds = MakeShareable(new FUICommandList);
	Cmds->MapAction(FGdhCommands::Get().ScanAssets, FExecuteAction::CreateLambda([&]()
	{
		UpdateListData();
		UpdateListView();
	}));

	Cmds->MapAction(FGdhCommands::Get().RenameAssets, FExecuteAction::CreateLambda([]() { UE_LOG(LogTemp, Warning, TEXT("Rename")) }));
	Cmds->MapAction(FGdhCommands::Get().ClearSelection, FExecuteAction::CreateLambda([&]()
	{
		UpdateListView();
	}));

	FPropertyEditorModule& PropertyEditor = UGdhSubsystem::GetModulePropertyEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bAllowFavoriteSystem = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = "GdhAssetNamingToolSettings";

	const auto SettingsProperty = PropertyEditor.CreateDetailView(DetailsViewArgs);
	SettingsProperty->SetObject(AssetNamingToolSettings.Get());

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
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot().Value(0.4f)
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
				+ SVerticalBox::Slot().Padding(5.0f).AutoHeight()
				[
					SettingsProperty
				]
			]
			+ SSplitter::Slot().Value(0.6f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(5.0f).FillHeight(1.0f)
				[
					SNew(SWidgetSwitcher)
					.WidgetIndex_Raw(this, &SGdhAssetNamingTool::GetWidgetIndex)
					+ SWidgetSwitcher::Slot()
					[
						SNew(SScrollBox)
						.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
						.AnimateWheelScrolling(true)
						.AllowOverscroll(EAllowOverscroll::No)
						+ SScrollBox::Slot()
						[
							SAssignNew(ListView, SListView<TWeakObjectPtr<UGdhAssetNamingToolListItem>>)
							.ListItemsSource(&ListItems)
							.SelectionMode(ESelectionMode::Multi)
							.OnGenerateRow(this, &SGdhAssetNamingTool::OnGenerateRow)
							.OnMouseButtonDoubleClick(this, &SGdhAssetNamingTool::OnListRowMouseDoubleClick)
							.HeaderRow(GetHeaderRow())
						]
					]
					+ SWidgetSwitcher::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right).VAlign(VAlign_Center)
						[
							SNew(SBox).WidthOverride(32.0f).HeightOverride(32.0f)
							[
								SNew(SImage).Image(FGdhStyles::GetIcon("GamedevHelper.Icon.Check").GetIcon())
							]
						]
						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(FMargin{5.0f, 2.0f, 0.0f, 0.0f}).HAlign(HAlign_Left).VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.ShadowOffset(FVector2D{1.5f, 1.5f})
							.ShadowColorAndOpacity(FLinearColor::Black)
							.Font(FGdhStyles::GetFont("Bold", 18))
							.Text(FText::FromString(TEXT("No assets to rename")))
							.ColorAndOpacity(FGdhStyles::Get().GetColor("GamedevHelper.Color.Grey"))
						]
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(5.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Left).VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text_Raw(this, &SGdhAssetNamingTool::GetListSummaryTxt)
					]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text_Raw(this, &SGdhAssetNamingTool::GetListSelectionTxt)
					]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right).VAlign(VAlign_Center)
					[
						SNew(SComboButton)
						.ContentPadding(0)
						.ForegroundColor_Raw(this, &SGdhAssetNamingTool::GetListOptionsBtnForegroundColor)
						.ButtonStyle(FEditorStyle::Get(), "ToggleButton")
						.OnGetMenuContent(this, &SGdhAssetNamingTool::GetListOptionsBtnContent)
						.ButtonContent()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
							[
								SNew(SImage).Image(FEditorStyle::GetBrush("GenericViewButton"))
							]
							+ SHorizontalBox::Slot().AutoWidth().Padding(2.0f, 0.0f, 0.0f, 0.0f).VAlign(VAlign_Center)
							[
								SNew(STextBlock).Text(FText::FromString(TEXT("View Options")))
							]
						]
					]
				]
			]
		]
	];
}

void SGdhAssetNamingTool::UpdateListData()
{
	if (UGdhSubsystem::GetModuleAssetRegistry().GetRegistry().IsLoadingAssets()) return;

	FScopedSlowTask SlowTask(
		2.0f,
		FText::FromString("Scanning assets")
	);
	SlowTask.MakeDialog(false, false);
	SlowTask.EnterProgressFrame(1.0f);

	TArray<FAssetData> AssetsAll;

	if (AssetNamingToolSettings->ScanPath.Path.IsEmpty())
	{
		UGdhSubsystem::GetAssetsAll(AssetsAll);
	}
	else
	{
		UGdhSubsystem::GetAssetsByPath(AssetNamingToolSettings->ScanPath.Path, AssetNamingToolSettings->bScanRecursive, AssetsAll);
	}

	TArray<FAssetData> AssetsIndirect;
	TArray<FGdhAssetIndirectInfo> AssetIndirectInfos;
	UGdhSubsystem::GetAssetsIndirect(AssetsIndirect, AssetIndirectInfos, true);

	ListItems.Reset(AssetsAll.Num());

	SlowTask.EnterProgressFrame(1.0f);

	FScopedSlowTask SlowTaskAssets(
		AssetsAll.Num(),
		FText::FromString("")
	);
	SlowTaskAssets.MakeDialog(false, false);

	for (const auto& Asset : AssetsAll)
	{
		SlowTaskAssets.EnterProgressFrame(1.0f);

		UGdhAssetNamingToolListItem* NewItem = NewObject<UGdhAssetNamingToolListItem>();
		if (!NewItem) continue;

		const FString AssetNameOriginal = Asset.AssetName.ToString();
		const FString AssetNamePreview = UGdhSubsystem::GetAssetRenamePreview(Asset);
		// const FString AssetPreviewObjectPath = Asset.PackagePath.ToString() + FString::Printf(TEXT("/%s.%s"), *AssetNamePreview, *AssetNamePreview);
		// const FAssetData ExistingAssetData = UGdhSubsystem::GetAssetByObjectPath(AssetPreviewObjectPath);

		// Determine if the asset should and can be renamed.
		const bool ShouldRenameAsset = !AssetNameOriginal.Equals(AssetNamePreview);
		// const bool AssetNameIsNotEmpty = !AssetNamePreview.IsEmpty();
		// const bool AssetIsNotIndirectlyReferenced = !AssetsIndirect.Contains(Asset);
		// const bool AssetHasNoNamingConflict = !ExistingAssetData.IsValid() || Asset.ObjectPath.ToString().Equals(ExistingAssetData.ObjectPath.ToString());
		// // const bool AssetIsNotIgnored = !AssetNamingConvention->AssetsIgnore.Contains(Asset.GetAsset());
		// const bool CanRenameAsset = AssetNameIsNotEmpty && AssetIsNotIndirectlyReferenced && AssetHasNoNamingConflict && AssetIsNotIgnored;

		if (!ShouldRenameAsset) continue;

		NewItem->AssetData = Asset;
		NewItem->OldName = AssetNameOriginal;
		NewItem->NewName = AssetNamePreview;

		ListItems.Emplace(NewItem);
		// if (bShowUnconfiguredAssets != CanRenameAsset)
		// {
		// }
	}
}

void SGdhAssetNamingTool::UpdateListView() const
{
	if (!ListView.IsValid()) return;

	ListView->ClearHighlightedItems();
	ListView->ClearSelection();
	ListView->RequestListRefresh();
}

void SGdhAssetNamingTool::UpdateListSort(const FName& ColumnName)
{
	if (!ListView.IsValid()) return;

	auto SortListItems = [&](auto& SortMode, auto SortFunc)
	{
		SortMode = SortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;

		ListItems.Sort(SortFunc);
	};

	if (ColumnName.IsEqual(TEXT("Preview")) || ColumnName.IsEqual(TEXT("AssetClass")))
	{
		SortListItems(ColumnSortModeClass, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModeClass == EColumnSortMode::Ascending ? Item1->AssetData.GetClass()->GetName() < Item2->AssetData.GetClass()->GetName() : Item1->AssetData.GetClass()->GetName() > Item2->AssetData.GetClass()->GetName();
		});
	}

	if (ColumnName.IsEqual(TEXT("Path")))
	{
		SortListItems(ColumnSortModeClass, [&](const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item1, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item2)
		{
			return ColumnSortModeClass == EColumnSortMode::Ascending
				       ? Item1->AssetData.ToSoftObjectPath().GetAssetPathString() < Item2->AssetData.ToSoftObjectPath().GetAssetPathString()
				       : Item1->AssetData.ToSoftObjectPath().GetAssetPathString() > Item2->AssetData.ToSoftObjectPath().GetAssetPathString();
		});
	}

	ListView->RebuildList();
}

void SGdhAssetNamingTool::OnListSort(EColumnSortPriority::Type SortPriority, const FName& ColumnName, EColumnSortMode::Type InSortMode) {}
void SGdhAssetNamingTool::OnListRowMouseDoubleClick(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item) {}

FText SGdhAssetNamingTool::GetListSummaryTxt() const
{
	return FText::FromString(FString::Printf(TEXT("Total %d assets"), ListItems.Num()));
}

FText SGdhAssetNamingTool::GetListSelectionTxt() const
{
	if (!ListView.IsValid()) return {};

	return FText::FromString(FString::Printf(TEXT("Selected %d"), ListView->GetSelectedItems().Num()));
}

FSlateColor SGdhAssetNamingTool::GetListOptionsBtnForegroundColor() const
{
	static const FName InvertedForegroundName("InvertedForeground");
	static const FName DefaultForegroundName("DefaultForeground");

	if (!ListOptionsBtn.IsValid()) return FEditorStyle::GetSlateColor(DefaultForegroundName);

	return ListOptionsBtn->IsHovered() ? FEditorStyle::GetSlateColor(InvertedForegroundName) : FEditorStyle::GetSlateColor(DefaultForegroundName);
}

TSharedRef<SWidget> SGdhAssetNamingTool::GetListOptionsBtnContent()
{
	const TSharedPtr<FExtender> Extender;
	FMenuBuilder MenuBuilder(true, Cmds, Extender, true);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Show Unconfigured Assets")),
		FText::FromString(TEXT("Show assets that dont have naming settings specified")),
		FSlateIcon(),
		FUIAction
		(
			FExecuteAction::CreateLambda([&]
			{
				// bShowUnconfiguredAssets = !bShowUnconfiguredAssets;
				//
				// UpdateListData();
				// UpdateListView();
				// UpdateListSort(TEXT("Preview"));
			}),
			FCanExecuteAction::CreateLambda([&]()
			{
				return true;
			}),
			FGetActionCheckState::CreateLambda([&]()
			{
				return ECheckBoxState::Unchecked;
				// return bShowUnconfiguredAssets ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
			})
		),
		NAME_None,
		EUserInterfaceActionType::ToggleButton
	);

	return MenuBuilder.MakeWidget();
}

TSharedRef<ITableRow> SGdhAssetNamingTool::OnGenerateRow(TWeakObjectPtr<UGdhAssetNamingToolListItem> Item, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGdhAssetNamingToolListItem, OwnerTable).ListItem(Item);
}

TSharedRef<SWidget> SGdhAssetNamingTool::CreateToolbarMain() const
{
	FToolBarBuilder ToolBarBuilder{Cmds, FMultiBoxCustomization::None};
	ToolBarBuilder.BeginSection("GdhAssetNamingToolMainToolbar");
	{
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().ScanAssets);
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().RenameAssets);
		ToolBarBuilder.AddSeparator();
		ToolBarBuilder.AddToolBarButton(FGdhCommands::Get().ClearSelection);
	}
	ToolBarBuilder.EndSection();

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SHeaderRow> SGdhAssetNamingTool::GetHeaderRow()
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
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
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
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
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
		  .FillWidth(0.4f)
		  .HeaderContentPadding(FMargin{5.0f})
		  .OnSort_Raw(this, &SGdhAssetNamingTool::OnListSort)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Path")))
			.ColorAndOpacity(FGdhStyles::Get().GetSlateColor("GamedevHelper.Color.Title"))
			.Font(FGdhStyles::GetFont("Light", 10.0f))
		];
}

int32 SGdhAssetNamingTool::GetWidgetIndex() const
{
	return ListItems.Num() == 0 ? GdhConstants::WidgetIndexWorking : GdhConstants::WidgetIndexIdle;
}
