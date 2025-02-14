// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "BatchEncodeTool/Slate/SGdhBetRenderListItem.h"
#include "LevelSequence.h"
#include "Widgets/Input/SHyperlink.h"

void SGdhBetRenderListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView) {
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow::Construct(SMultiColumnTableRow::FArguments().Padding(FMargin {0.0f, 2.0f, 0.0f, 0.0f}), InOwnerTableView);
}

TSharedRef<SWidget> SGdhBetRenderListItem::GenerateWidgetForColumn(const FName& InColumnName) {
	if (InColumnName == TEXT("Name")) {

		const TSharedPtr<FAssetThumbnail> AssetThumbnail = MakeShareable(new FAssetThumbnail(ListItem->Sequence, 16, 16, nullptr));

		FAssetThumbnailConfig ThumbnailConfig;
		ThumbnailConfig.bForceGenericThumbnail = true;

		// clang-format off
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth().Padding(FMargin{10.0f, 0.0f, 5.0f, 0.0f})
			[
				SNew(SBox).WidthOverride(16).HeightOverride(16)
				[
					AssetThumbnail->MakeThumbnailWidget(ThumbnailConfig)
				]
			]
			+ SHorizontalBox::Slot().VAlign(VAlign_Center).FillWidth(1.0f).Padding(FMargin{5.0f, 0.0f, 0.0f, 0.0f})
			[
				SNew(STextBlock).Text(FText::FromString(ListItem->Name)).Justification(ETextJustify::Left)
			];
		// clang-format on
	}

	if (InColumnName == TEXT("FrameStart")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->FrameStart)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FrameEnd")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->FrameEnd)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("FrameRate")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->FrameRate)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("DurationFrames")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->DurationFrames)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("DurationHuman")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->DurationHuman)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("TrackSlomo")) {
		return SNew(STextBlock).Text(FText::FromString(ListItem->HasTrackSlomo)).Justification(ETextJustify::Center);
	}

	if (InColumnName == TEXT("EncodeCmdPreview")) {
		// clang-format off
		return
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(FMargin {5.0f, 0.0f})
			[
				SNew(SHyperlink)
				.Text(FText::FromName(TEXT("Show")))
				.ToolTipText(FText::FromString(ListItem->EncodeCmdPreview))
				.OnNavigate_Lambda([&]() {
					TSharedRef<SWindow> WindowEncodeCmd =
						SNew(SWindow)
						.Title(FText::FromString(TEXT("Encode Command Preview")))
						.ClientSize(FVector2D {1280, 720})
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().FillHeight(1.0f).Padding(5.0f)
							[
								SNew(SEditableText).IsReadOnly(true).Text(FText::FromString(ListItem->EncodeCmdPreview)).Justification(ETextJustify::Left)
							]
						];

					if (GEditor) {
						GEditor->EditorAddModalWindow(WindowEncodeCmd);
					}
				})
			];
		// clang-format on
	}

	return SNew(STextBlock).Text(FText::FromString(TEXT("")));
}
