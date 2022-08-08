// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/Renderer/GamedevHelperRendererQueueItemUI.h"
#include "UI/Renderer/GamedevHelperRendererQueueItem.h"
#include "UI/GamedevHelperEditorStyle.h"
// Engine Headers
#include "Widgets/Input/SHyperlink.h"
#include "MoviePipelineQueueSubsystem.h"

void SGamedevHelperRendererQueueItemUI::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	QueueItem = InArgs._QueueItem;

	SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRendererQueueItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRendererQueueItem>>::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGamedevHelperRendererQueueItemUI::GenerateWidgetForColumn(const FName& InColumnName)
{
	if (InColumnName == TEXT("State"))
	{
		const FString Icon = FGamedevHelperEditorStyle::GetIconByStatus(QueueItem->Status);
		return SNew(SBox)
			.WidthOverride(20)
			.HeightOverride(20)
		[
			SNew(SImage)
			.Image(FGamedevHelperEditorStyle::GetIcon(Icon))
		];
	}

	if (InColumnName == TEXT("PipelineName"))
	{
		return SNew(STextBlock).Text(FText::FromString(QueueItem->PipelineName));
	}

	if (InColumnName == TEXT("ShotName"))
	{
		return SNew(STextBlock).Text(FText::FromString(QueueItem->SequenceName));
	}

	// if (InColumnName == TEXT("ShotFrameStart"))
	// {
	// 	return SNew(STextBlock).Text(FText::FromString(FString::FromInt(QueueItem->ShotStartFrame)));
	// }
	//
	// if (InColumnName == TEXT("ShotDuration"))
	// {
	// 	return SNew(STextBlock).Text(FText::FromString(QueueItem->ShotDuration));
	// }
	//
	// if (InColumnName == TEXT("ShotOutputDir"))
	// {
	// 	return SNew(SBox)
	// 	.VAlign(VAlign_Center)
	// 	.HAlign(HAlign_Left)
	// 	.ToolTipText(FText::FromString(QueueItem->ShotOutputDir))
	// 	[
	// 		SNew(SHyperlink)
	// 		.Text(FText::FromString(QueueItem->ShotOutputDir))
	// 		.OnNavigate_Lambda([=]()
	// 		{
	// 			// ignore if we currently rendering sequences
	// 			if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
	// 			{
	// 				return;
	// 			}
	// 			
	// 			FPlatformProcess::ExploreFolder(*QueueItem->ShotOutputDir);
	// 		})
	// 	];
	// }
	//
	// if (InColumnName == TEXT("ShotRenderedFrames"))
	// {
	// 	return SNew(STextBlock).Text(FText::FromString(FString::FromInt(QueueItem->ShotRenderedFrames)));
	// }
	//
	// if (InColumnName == TEXT("Note"))
	// {
	// 	const FLinearColor Color = UGamedevHelperEditorStyle::GetColorByStatus(QueueItem->Status);
	// 	
	// 	return SNew(STextBlock).ColorAndOpacity(Color).Text(FText::FromString(QueueItem->Note));
	// }

	return SNew(STextBlock).Text(FText::FromString(""));
}
