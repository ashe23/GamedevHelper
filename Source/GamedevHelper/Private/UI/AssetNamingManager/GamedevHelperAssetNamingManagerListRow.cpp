// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerListRow.h"
#include "UI/GamedevHelperEditorStyle.h"
#include "AssetThumbnail.h"

void SGamedevHelperAssetNamingListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	RowItem = InArgs._RowItem;

	SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperAssetNamingListItem>>::FArguments()
		// .Style(FEditorStyle::Get(), "ContentBrowser.AssetListView.TableRow")
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGamedevHelperAssetNamingListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Selection"))
	{
		return SNew(SCheckBox);
	}
	if (InColumnName == TEXT("AssetClass"))
	{
		return SNew(STextBlock).Text(FText::FromString(RowItem->AssetData.AssetClass.ToString()));
	}
	if (InColumnName == TEXT("Preview"))
	{
		const TSharedPtr<FAssetThumbnail> AssetThumbnail = MakeShareable(new FAssetThumbnail(RowItem->AssetData, 16, 16, nullptr));
		const FAssetThumbnailConfig ThumbnailConfig;

		return SNew(SBox)
			.WidthOverride(16)
			.HeightOverride(16)
			[
				AssetThumbnail->MakeThumbnailWidget(ThumbnailConfig)
			];
	}
	if (InColumnName == TEXT("Result"))
	{
		return
			SNew(SBox)
			.Padding(FMargin{5.0f, 0.0f})
			// .HAlign(HAlign_Fill)
			// .VAlign(VAlign_Fill)
			[
				
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGamedevHelperEditorStyle::GetIcon(TEXT("GamedevHelper.Icon.BG.16")))
					.BorderBackgroundColor(FLinearColor{FColor::FromHex(TEXT("#C62828"))})
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(RowItem->OldName))
					]
				]
				+SHorizontalBox::Slot()
				.Padding(FMargin{5.0f, 0.0f})
				.AutoWidth()
				[
					SNew(SImage)
					.ColorAndOpacity(FLinearColor{FColor::FromHex(TEXT("#616161"))})
					.Image(FGamedevHelperEditorStyle::GetIcon(TEXT("GamedevHelper.Icon.ArrowRight.16")))
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGamedevHelperEditorStyle::GetIcon(TEXT("GamedevHelper.Icon.BG.16")))
					.BorderBackgroundColor(FLinearColor{FColor::FromHex(TEXT("#2E7D32"))})
					
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(RowItem->NewName))
					]
				]
			];
	}

	if (InColumnName == TEXT("Path"))
	{
		return SNew(STextBlock).Justification(ETextJustify::Left).Text(FText::FromString(RowItem->AssetData.PackagePath.ToString()));
	}

	return SNew(STextBlock).Text(FText::FromString(""));
}