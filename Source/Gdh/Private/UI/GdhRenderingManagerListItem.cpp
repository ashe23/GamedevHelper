// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/GdhRenderingManagerListItem.h"
#include "GdhStyles.h"
#include "Settings/GdhRenderingSettings.h"
#include "Libs/GdhTimeLibrary.h"

void SGdhRenderingManagerListItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	ListItem = InArgs._ListItem;

	SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderingManagerListItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderingManagerListItem>>::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGdhRenderingManagerListItem::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName.IsEqual(TEXT("Name")))
	{
		return SNew(STextBlock).Text(FText::FromString(ListItem->Name));
	}

	if (InColumnName.IsEqual(TEXT("Duration")))
	{
		const FString Duration = FString::Printf(
			TEXT("%d frames (%s%s)"),
			ListItem->DurationInFrames,
			ListItem->bHasTimeDilationTrack ? TEXT("~") : TEXT(""),
			*UGdhTimeLibrary::GetHumanReadableTime(ListItem->DurationInSeconds));

		return SNew(STextBlock).Text(FText::FromString(Duration));
	}

	if (InColumnName.IsEqual(TEXT("FrameStart")))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("%d"), ListItem->FrameStart)));
	}

	if (InColumnName.IsEqual(TEXT("RenderedFrames")))
	{
		EGdhGenericStatus Status;

		if (ListItem->RenderedFramesNum == 0 || ListItem->bHasMissingFrames)
		{
			Status = EGdhGenericStatus::Error;
		}
		else if (ListItem->bHasTimeDilationTrack)
		{
			Status = EGdhGenericStatus::Warning;
		}
		else
		{
			Status = ListItem->RenderedFramesNum == ListItem->DurationInFrames ? EGdhGenericStatus::OK : EGdhGenericStatus::Error;
		}

		const FLinearColor Color = FGdhStyles::GetColorByStatus(Status);
		const FString Str = FString::Printf(
			TEXT("%d of %d%s"),
			ListItem->RenderedFramesNum,
			ListItem->DurationInFrames,
			ListItem->bHasTimeDilationTrack ? TEXT(" (Has Slomo Track)") : TEXT("")
		);
		return SNew(STextBlock).ColorAndOpacity(Color).Text(FText::FromString(Str));
	}

	if (InColumnName.IsEqual(TEXT("VideoEncoded")))
	{
		const FString Icon = FGdhStyles::GetIconByStatus(ListItem->bVideoEncoded ? EGdhGenericStatus::OK : EGdhGenericStatus::Error);
		const FString Msg = ListItem->bVideoEncoded ? TEXT("") : TEXT("Missing encoded video");
		
		return SNew(SImage).ToolTipText(FText::FromString(Msg)).Image(FGdhStyles::GetIcon(Icon));
	}

	if (InColumnName.IsEqual(TEXT("Note")))
	{
		const FLinearColor Color = FGdhStyles::GetColorByStatus(ListItem->NoteStatus);
		return SNew(STextBlock).ColorAndOpacity(Color).Text(FText::FromString(ListItem->Note));
	}

	return SNew(STextBlock).Text(FText::FromString(""));
}
