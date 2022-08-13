// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerQueueItemUI.generated.h"

UCLASS(Transient)
class UGamedevHelperRenderingManagerQueueItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	EGamedevHelperRendererStatus Status = EGamedevHelperRendererStatus::None;

	UPROPERTY()
	FString QueueName;
	
	UPROPERTY()
	FString SequenceName;
	
	UPROPERTY()
	FString SequenceDuration;
	
	UPROPERTY()
	int32 SequenceDurationInFrames = 0;
	
	UPROPERTY()
	int32 SequenceStartFrame = 0;
	
	UPROPERTY()
	int32 SequenceRenderedFrames = 0;
	
	UPROPERTY()
	FString Note;
};

class SGamedevHelperRenderingManagerQueueItemUI : public SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRenderingManagerQueueItemUI){}
		SLATE_ARGUMENT(TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>, QueueItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem> QueueItem;
};


