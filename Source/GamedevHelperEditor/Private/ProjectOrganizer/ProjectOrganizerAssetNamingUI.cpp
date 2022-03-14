// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectOrganizer/ProjectOrganizerAssetNamingUI.h"
#include "ProjectOrganizer/ProjectOrganizerAssetNamingListRow.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"
#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

void SProjectOrganizerAssetNamingUI::Construct(const FArguments& InArgs)
{
	SortColumn = TEXT("AssetClass");

	ListUpdate();

	ChildSlot
	[
		SNew(SScrollBox)
		.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
		.AnimateWheelScrolling(true)
		.AllowOverscroll(EAllowOverscroll::No)
		+SScrollBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(FMargin{10.0f, 0.0f})
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SScrollBox)
				.ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
				.AnimateWheelScrolling(true)
				.AllowOverscroll(EAllowOverscroll::No)
				+SScrollBox::Slot()
				[
					SAssignNew(ListView, SListView<TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>>)
					.ListItemsSource(&AssetList)
					.SelectionMode(ESelectionMode::Multi)
					.OnGenerateRow(this, &SProjectOrganizerAssetNamingUI::OnGenerateRow)
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
						.OnSort_Raw(this, &SProjectOrganizerAssetNamingUI::OnSort)
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
						.OnSort_Raw(this, &SProjectOrganizerAssetNamingUI::OnSort)
						.HAlignCell(HAlign_Left)
						.VAlignCell(VAlign_Center)
						.HAlignHeader(HAlign_Center)
						.HeaderContentPadding(FMargin(10.0f))
						.FillWidth(0.4f)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Result", "Name Fix Preview"))
						]
						+ SHeaderRow::Column(FName("Path"))
						.OnSort_Raw(this, &SProjectOrganizerAssetNamingUI::OnSort)
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
	];
}

void SProjectOrganizerAssetNamingUI::ListUpdate()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	
	FARFilter Filter;
	Filter.PackagePaths.Add("/Game");
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
		UProjectOrganizerAssetNamingListRowData* Data = NewObject<UProjectOrganizerAssetNamingListRowData>();
		Data->OldName = Asset.AssetName.ToString();
		Data->NewName = TEXT("New_") + Asset.AssetName.ToString();

		if (Data->OldName.Equals(Data->NewName)) continue;
		
		Data->AssetData = Asset;
	
		AssetList.Add(Data);
	}

	ListSort();
}

void SProjectOrganizerAssetNamingUI::ListSort()
{
	// todo:ashe23 fix sorting, when just updating naming case, list must not change its order
	
	if (CurrentSortMode == EColumnSortMode::Ascending)
	{
		AssetList.Sort([&](const TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>& Data1, const TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare( Data2->AssetData.AssetName) < 0; 
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare( Data2->AssetData.PackagePath) < 0; 
			}

			return Data1->AssetData.AssetClass.Compare( Data2->AssetData.AssetClass) < 0; 
		});
	}

	if (CurrentSortMode == EColumnSortMode::Descending)
	{
		AssetList.Sort([&](const TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>& Data1, const TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData>& Data2)
		{
			if (SortColumn.IsEqual(TEXT("Result")))
			{
				return Data1->AssetData.AssetName.Compare( Data2->AssetData.AssetName) > 0;
			}

			if (SortColumn.IsEqual(TEXT("Path")))
			{
				return Data1->AssetData.PackagePath.Compare( Data2->AssetData.PackagePath) > 0; 
			}

			return Data1->AssetData.AssetClass.Compare( Data2->AssetData.AssetClass) > 0;
		});
	}
}

TSharedRef<ITableRow> SProjectOrganizerAssetNamingUI::OnGenerateRow(TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SProjectOrganizerAssetNamingListRow, OwnerTable).RowItem(InItem);
}

void SProjectOrganizerAssetNamingUI::OnSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode)
{
	CurrentSortMode =  CurrentSortMode == EColumnSortMode::Ascending ? EColumnSortMode::Descending : EColumnSortMode::Ascending;
	SortColumn = Name;
	
	ListUpdate();
	
	if (ListView.IsValid())
	{
		ListView->RebuildList();
	}
}

#undef LOCTEXT_NAMESPACE
