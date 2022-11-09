// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhAssetNamingManagerListItem.h"
#include "GdhStyles.h"

void SGdhAssetNamingManagerListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow<TWeakObjectPtr<UGdhAssetNamingManagerListItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGdhAssetNamingManagerListItem>>::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhAssetNamingManagerListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Preview"))
	{
		const TSharedPtr<FAssetThumbnail> AssetThumbnail = MakeShareable(new FAssetThumbnail(ListItem->AssetData, 16, 16, nullptr));
		const FAssetThumbnailConfig ThumbnailConfig;

		return
			SNew(SBox)
			.WidthOverride(16)
			.HeightOverride(16)
			[
				AssetThumbnail->MakeThumbnailWidget(ThumbnailConfig)
			];
	}

	if (InColumnName == TEXT("AssetClass"))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->AssetData.AssetClass.ToString()));
	}

	if (InColumnName == TEXT("Result"))
	{
		return
			SNew(SBox)
			.Padding(FMargin{5.0f, 0.0f})
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGdhStyles::GetIcon(TEXT("GamedevHelper.Icon.BG.16")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red")))
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(ListItem->OldName))
					]
				]
				+ SHorizontalBox::Slot()
				  .Padding(FMargin{5.0f, 0.0f})
				  .AutoWidth()
				[
					SNew(SImage)
					.ColorAndOpacity(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Grey")))
					.Image(FGdhStyles::GetIcon(TEXT("GamedevHelper.Icon.ArrowRight.16")))
				]
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGdhStyles::GetIcon(TEXT("GamedevHelper.Icon.BG.16")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Green")))
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(ListItem->NewName))
					]
				]
			];
	}

	if (InColumnName == TEXT("Path"))
	{
		return SNew(STextBlock).Justification(ETextJustify::Left).Text(FText::FromString(ListItem->AssetData.PackagePath.ToString()));
	}

	if (InColumnName == TEXT("Note"))
	{
		const FSlateColor Color = ListItem->RenameStatus == EGdhRenameStatus::OkToRename
			                          ? FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Green"))
			                          : FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red"));
	
		return SNew(STextBlock).Justification(ETextJustify::Center)
		                       .ColorAndOpacity(Color)
		                       .Text(FText::FromString(ListItem->Note));
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
