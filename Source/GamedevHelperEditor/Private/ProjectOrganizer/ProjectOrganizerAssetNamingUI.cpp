// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectOrganizer/ProjectOrganizerAssetNamingUI.h"
#include "ProjectOrganizer/ProjectOrganizerAssetNamingListRow.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "FGamedevHelperEditor"

void SProjectOrganizerAssetNamingUI::Construct(const FArguments& InArgs)
{
	SortColumn = TEXT("AssetClass");

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
						// .OnSort_Raw(this, &SProjectOrganizerWindowUI::OnSort)
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
						// .OnSort_Raw(this, &SProjectOrganizerWindowUI::OnSort)
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
						// .OnSort_Raw(this, &SProjectOrganizerWindowUI::OnSort)
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
}

void SProjectOrganizerAssetNamingUI::ListSort()
{
}

TSharedRef<ITableRow> SProjectOrganizerAssetNamingUI::OnGenerateRow(TWeakObjectPtr<UProjectOrganizerAssetNamingListRowData> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SProjectOrganizerAssetNamingListRow, OwnerTable).RowItem(InItem);
}

#undef LOCTEXT_NAMESPACE