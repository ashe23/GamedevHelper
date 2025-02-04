// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhBetQueueItem.generated.h"

class ULevelSequence;

UCLASS(Transient)
class UGdhBetQueueItem : public UObject
{
public:

	GENERATED_BODY()

	FString NameRenderList;
	FString NameSequence;
	bool bDragRow = false;
	TArray<FString> Presets;
	TArray<FString> AudioTracks;

	UPROPERTY()
	const ULevelSequence* LevelSequence = nullptr;
};

class SGdhBetQueueItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhBetQueueItem>>
{
public:

	SLATE_BEGIN_ARGS(SGdhBetQueueItem) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UGdhBetQueueItem>, QueueItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
	FReply OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);
	bool CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);

private:

	TWeakObjectPtr<UGdhBetQueueItem> QueueItem;
};
