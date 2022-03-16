// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerWindowUI.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerListRow.h"
#include "UI/GamedevHelperEditorStyle.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"
#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "FGamedevHelper"

void SAssetNamingManagerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	Settings = GetMutableDefault<UGamedevHelperAssetNamingManagerSettings>();
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
						.ButtonColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#9E9E9E"))})
						.ContentPadding(FMargin{0})
						// .OnClicked_Raw(this, &SProjectOrganizerWindowUI::OnRefreshBtnClick)
						[
							SNew(STextBlock)
							.Font(FGamedevHelperEditorStyle::Get().GetFontStyle("GamedevHelper.Font.Light20"))
							.Text(FText::FromString(TEXT("Fix Names")))
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
								.Text(LOCTEXT("Preview", "Type"))
							]
							+ SHeaderRow::Column(FName("AssetClass"))
							  .OnSort_Raw(this, &SAssetNamingManagerWindow::OnSort)
							  .HAlignCell(HAlign_Center)
							  .VAlignCell(VAlign_Center)
							  .HAlignHeader(HAlign_Center)
							  .HeaderContentPadding(FMargin(10.0f))
							  .FixedWidth(150.0f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("AssetClass", "Class"))
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
							  .FillWidth(0.6f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("Path", "Path"))
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
	Filter.bRecursivePaths = true;
	// for (const auto Prefix : Settings->AssetPrefixes)
	// {
	// 	if (!Prefix.Key) continue;
	// 	const FString ClassName = Prefix.Key->GetName();
	// 	if (ClassName.IsEmpty()) continue;
	// 	
	// 	Filter.ClassNames.Add(FName{*ClassName});
	// }

	TArray<FAssetData> Assets;
	AssetRegistryModule.Get().GetAssets(Filter, Assets);

	AssetList.Reset();
	AssetList.Reserve(Assets.Num());

	for (const auto& Asset : Assets)
	{
		UGamedevHelperAssetNamingListItem* Data = NewObject<UGamedevHelperAssetNamingListItem>();
		Data->OldName = Asset.AssetName.ToString();
		Data->NewName = TEXT("New_") + Asset.AssetName.ToString(); // todo:ashe23

		if (Data->OldName.Equals(Data->NewName)) continue;

		Data->AssetData = Asset;

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

TSharedRef<ITableRow> SAssetNamingManagerWindow::OnGenerateRow(TWeakObjectPtr<UGamedevHelperAssetNamingListItem> InItem,
                                                               const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGamedevHelperAssetNamingListItem, OwnerTable).RowItem(InItem);
}

FReply SAssetNamingManagerWindow::OnRefreshBtnClick()
{
	ListUpdate();

	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}

	return FReply::Handled();
}

void SAssetNamingManagerWindow::OnSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode)
{
	CurrentSortMode = CurrentSortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;
	SortColumn = Name;

	ListUpdate();

	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
}

#undef LOCTEXT_NAMESPACE
