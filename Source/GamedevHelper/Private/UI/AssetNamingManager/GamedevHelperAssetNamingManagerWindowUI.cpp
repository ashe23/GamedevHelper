// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerWindowUI.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerListRow.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "GamedevHelper.h"
#include "GamedevHelperAssetNamingManagerLibrary.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"
#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

void SAssetNamingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	Settings = GetMutableDefault<UGamedevHelperAssetNamingManagerSettings>();
	Settings->OnSettingsChangeDelegate.BindRaw(this, &SAssetNamingManagerWindow::ListRefresh);

	FDetailsViewArgs ViewArgs;
	ViewArgs.bUpdatesFromSelection = false;
	ViewArgs.bLockable = false;
	ViewArgs.bShowScrollBar = true;
	ViewArgs.bAllowSearch = false;
	ViewArgs.bShowOptions = false;
	ViewArgs.bAllowFavoriteSystem = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.ViewIdentifier = "AssetNamingManagerSettings";


	const TSharedPtr<IDetailsView> SettingsProperty = PropertyEditor.CreateDetailView(ViewArgs);
	SettingsProperty->SetObject(Settings);

	SortColumn = TEXT("AssetClass");

	ListUpdate();

	ChildSlot
	[
		SNew(SSplitter)
		.Style(FEditorStyle::Get(), "ContentBrowser.Splitter")
		.Orientation(Orient_Horizontal)
		.PhysicalSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(0.4f)
		[
			SNew(SScrollBox)
			.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
			.AnimateWheelScrolling(true)
			.AllowOverscroll(EAllowOverscroll::No)
			+ SScrollBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					SettingsProperty.ToSharedRef()
				]
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#4CAF50"))})
						.ContentPadding(FMargin{0})
						.OnClicked_Raw(this, &SAssetNamingManagerWindow::OnRenameBtnClick)
						.IsEnabled_Raw(this, &SAssetNamingManagerWindow::IsRenameBtnEnabled)
						[
							SNew(STextBlock)
							.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light20"))
							.Text(FText::FromString(TEXT("Rename Assets")))
						]
					]
					+ SHorizontalBox::Slot()
					  .HAlign(HAlign_Fill)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#9E9E9E"))})
						.ContentPadding(FMargin{0})
						.OnClicked_Raw(this, &SAssetNamingManagerWindow::OnRefreshBtnClick)
						[
							SNew(STextBlock)
							.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light20"))
							.Text(FText::FromString(TEXT("Refresh")))
						]
					]
				]
			]
		]
		+ SSplitter::Slot()
		.Value(0.6f)
		[
			SNew(SScrollBox)
			.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
			.AnimateWheelScrolling(true)
			.AllowOverscroll(EAllowOverscroll::No)
			+ SScrollBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .Padding(FMargin{10.0f})
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				  .AutoHeight()
				[
					SNew(SScrollBox)
					.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
					.AnimateWheelScrolling(true)
					.AllowOverscroll(EAllowOverscroll::No)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListView, SListView<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>)
						.ListItemsSource(&AssetList)
						.SelectionMode(ESelectionMode::Multi)
						.OnGenerateRow(this, &SAssetNamingManagerWindow::OnGenerateRow)
						// .OnContextMenuOpening(this, &SVirtualGamesRendererWindow::OnContextMenu)
						// .OnMouseButtonDoubleClick_Raw(this, &SVirtualGamesRendererWindow::OnRowMouseDblClick)
						.HeaderRow
						(
							SNew(SHeaderRow)
							+ SHeaderRow::Column(FName("Preview"))
							  .HAlignCell(HAlign_Center)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FixedWidth(50.0f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Preview", "#"))
							]
							+ SHeaderRow::Column(FName("AssetClass"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Center)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FixedWidth(200.0f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("AssetClass", "Type"))
							]
							+ SHeaderRow::Column(FName("Result"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Left)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FillWidth(0.4f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Result", "Naming Preview"))
							]
							+ SHeaderRow::Column(FName("Path"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Left)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FillWidth(0.3f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Path", "Path"))
							]
							+ SHeaderRow::Column(FName("Note"))
							  // .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Left)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FillWidth(0.3f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Note", "Note"))
							]
						)
					]
				]
			]
		]
	];
}

void SAssetNamingManagerWindow::ListUpdate()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

	FARFilter Filter;
	Filter.PackagePaths.Add(FName{*Settings->ScanPath.Path});
	Filter.bRecursivePaths = Settings->bScanRecursive;

	// todo:ashe23 filter assets by class if settings enabled

	TArray<FAssetData> Assets;
	AssetRegistryModule.Get().GetAssets(Filter, Assets);

	RenamePreviews.Reset();
	RenamePreviews.Reserve(Assets.Num());
	for (const auto& Asset : Assets)
	{
		// todo:ashe23 refactor this part
		FGamedevHelperRenamePreview RenamePreview = UGamedevHelperAssetNamingManagerLibrary::GetRenamePreview(Asset, Settings);

		if (!RenamePreview.bValid && Settings->bShowMissingTypes)
		{
			RenamePreviews.Add(RenamePreview);
			continue;
		}
		
		// check if rename preview has conflicting names
		const auto OtherPreview = RenamePreviews.FindByPredicate([&](const FGamedevHelperRenamePreview& Prev)
		{
			return Prev.NewName.Equals(RenamePreview.NewName, ESearchCase::CaseSensitive);
		});

		if (OtherPreview)
		{
			OtherPreview->bValid = false;
			OtherPreview->ErrMsg = TEXT("Asset with same name already exists in previews");

			RenamePreview.bValid = false;
			RenamePreview.ErrMsg = TEXT("Asset with same name already exists in previews");

			RenamePreviews.Add(RenamePreview);
			continue;
		}
		
		if (RenamePreview.bValid)
		{
			RenamePreviews.Add(RenamePreview);
		}
	}

	AssetList.Reset();
	AssetList.Reserve(RenamePreviews.Num());

	for (const auto& RenamePreview : RenamePreviews)
	{
		UGamedevHelperAssetNamingListItem* Data = NewObject<UGamedevHelperAssetNamingListItem>();

		Data->OldName = RenamePreview.AssetData.AssetName.ToString();
		Data->NewName = RenamePreview.NewName;
		Data->AssetData = RenamePreview.AssetData;
		Data->Note = RenamePreview.ErrMsg;

		AssetList.Add(Data);
	}

	ListSort();
}

void SAssetNamingManagerWindow::ListSort()
{
	// todo:ashe23 fix sorting, when just updating naming case, list must not change its order

	if (CurrentSortMode == EColumnSortMode::Ascending)
	{
		AssetList.Sort([&](const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data1,
		                   const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare(Data2->AssetData.AssetName) < 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare(Data2->AssetData.PackagePath) < 0;
			}

			return Data1->AssetData.AssetClass.Compare(Data2->AssetData.AssetClass) < 0;
		});
	}

	if (CurrentSortMode == EColumnSortMode::Descending)
	{
		AssetList.Sort([&](const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data1,
		                   const TWeakObjectPtr<UGamedevHelperAssetNamingListItem>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare(Data2->AssetData.AssetName) > 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare(Data2->AssetData.PackagePath) > 0;
			}

			return Data1->AssetData.AssetClass.Compare(Data2->AssetData.AssetClass) > 0;
		});
	}
}

void SAssetNamingManagerWindow::ListRefresh()
{
	ListUpdate();

	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
}

TSharedRef<ITableRow> SAssetNamingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGamedevHelperAssetNamingListItem> InItem,
                                                               const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperAssetNamingListItem, OwnerTable).RowItem(InItem);
}

FReply SAssetNamingManagerWindow::OnRenameBtnClick()
{
	TArray<FAssetData> AssetsList;
	AssetsList.Reserve(AssetList.Num());

	for (const auto& Asset : AssetList)
	{
		AssetsList.Add(Asset->AssetData);
	}

	UGamedevHelperAssetNamingManagerLibrary::RenameAssets(AssetsList);

	ListRefresh();

	return FReply::Handled();
}

FReply SAssetNamingManagerWindow::OnRefreshBtnClick()
{
	ListRefresh();

	return FReply::Handled();
}

bool SAssetNamingManagerWindow::IsRenameBtnEnabled() const
{
	for (const auto& RenamePreview : RenamePreviews)
	{
		if (!RenamePreview.bValid)
		{
			return false;
		}
	}

	return true;;
}

void SAssetNamingManagerWindow::OnSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode)
{
	CurrentSortMode = CurrentSortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;
	SortColumn = Name;

	ListRefresh();
}

#undef LOCTEXT_NAMESPACE
