// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueItemUI.h"
#include "UI/RenderingManager/GamedevHelperRenderingManagerQueueItem.h"
#include "UI/GamedevHelperEditorStyle.h"
// Engine Headers
#include "MoviePipelineQueueSubsystem.h"
#include "Widgets/Input/SHyperlink.h"

void SGamedevHelperRenderingManagerQueueItemUI::Construct(const FArguments& InArgs,const TSharedRef<STableViewBase>& InOwnerTableView)
{
	QueueItem = InArgs._QueueItem;
	
	SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>::Construct(
		SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>::FArguments()
		.Padding(FMargin{0.0f, 2.0f, 0.0f, 0.0f}),
		InOwnerTableView
	);
}

TSharedRef<SWidget> SGamedevHelperRenderingManagerQueueItemUI::GenerateWidgetForColumn(const FName& InColumnName)
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

	if (InColumnName == TEXT("QueueName"))
	{
		return SNew(STextBlock).Text(FText::FromString(QueueItem->QueueName));
	}

	if (InColumnName == TEXT("SequenceName"))
	{
		return SNew(STextBlock).Text(FText::FromString(QueueItem->SequenceName));
	}

	if (InColumnName == TEXT("SequenceFrameStart"))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::FromInt(QueueItem->SequenceStartFrame)));
	}
	
	if (InColumnName == TEXT("SequenceDuration"))
	{
		return SNew(STextBlock).Text(FText::FromString(QueueItem->SequenceDuration));
	}
	
	if (InColumnName == TEXT("SequenceOutputDir"))
	{
		return SNew(SBox)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		.ToolTipText(FText::FromString(QueueItem->SequenceOutputDir))
		[
			SNew(SHyperlink)
			.Text(FText::FromString(QueueItem->SequenceOutputDir))
			.OnNavigate_Lambda([=]()
			{
				// ignore if we currently rendering sequences
				if (GEditor && GEditor->GetEditorSubsystem<UMoviePipelineQueueSubsystem>()->IsRendering())
				{
					return;
				}
				
				FPlatformProcess::ExploreFolder(*QueueItem->SequenceOutputDir);
			})
		];
	}
	
	if (InColumnName == TEXT("SequenceRenderedFrames"))
	{
		return SNew(STextBlock).Text(FText::FromString(FString::FromInt(QueueItem->SequenceRenderedFrames)));
	}
	
	if (InColumnName == TEXT("Note"))
	{
		const FLinearColor Color = FGamedevHelperEditorStyle::GetColorByStatus(QueueItem->Status);
		
		return SNew(STextBlock).ColorAndOpacity(Color).Text(FText::FromString(QueueItem->Note));
	}

	return SNew(STextBlock).Text(FText::FromString(""));
}