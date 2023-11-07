// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Slate/SGdhManagerAssetNamingItem.h"
#include "GdhStyles.h"
#include "GdhSubsystem.h"
#include "GdhTypes.h"
#include "Widgets/Input/SHyperlink.h"

void SGdhManagerAssetNamingItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	RowItem = InArgs._RowItem;

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhManagerAssetNamingItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("Preview"))
	{
		const TSharedPtr<FAssetThumbnail> AssetThumbnail = MakeShareable(new FAssetThumbnail(RowItem->AssetData, 16, 16, nullptr));
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
		return SNew(STextBlock).Text(FText::FromString(RowItem->AssetData.AssetClass.ToString()));
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
					.BorderImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Bg")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Red")))
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(FText::FromString(RowItem->OldName))
					]
				]
				+ SHorizontalBox::Slot()
				  .Padding(FMargin{5.0f, 0.0f})
				  .AutoWidth()
				[
					SNew(SImage)
					.Image(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Arrow")))
					.ColorAndOpacity(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Grey")))
				]
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FGdhStyles::GetIconBrush(TEXT("GamedevHelper.Icon.Bg")))
					.BorderBackgroundColor(FGdhStyles::GetColor(TEXT("GamedevHelper.Color.Green")))
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
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Left).VAlign(VAlign_Center).Padding(FMargin{5.0f, 0.0f})
			[
				SNew(SHyperlink)
				.Text(FText::FromString(RowItem->AssetData.PackagePath.ToString()))
				.OnNavigate_Lambda([&]() { UGdhSubsystem::OpenAssetInContentBrowser(RowItem->AssetData); })
			];
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
