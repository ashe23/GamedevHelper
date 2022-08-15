// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerListItem.generated.h"

UCLASS(Transient)
class UGamedevHelperRenderingManagerListItem : public UObject
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

	UPROPERTY()
	FSoftObjectPath QueueAsset;

	UPROPERTY()
	FSoftObjectPath LevelSequence;
};

class SGamedevHelperRenderingManagerListItem : public SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRenderingManagerListItem){}
		SLATE_ARGUMENT(TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>, QueueItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGamedevHelperRenderingManagerListItem> ListItem;
};


