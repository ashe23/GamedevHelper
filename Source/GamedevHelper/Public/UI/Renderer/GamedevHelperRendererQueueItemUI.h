// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UGamedevHelperRendererQueueItem;

class SGamedevHelperRendererQueueItemUI : public SMultiColumnTableRow<TWeakObjectPtr<UGamedevHelperRendererQueueItem>>
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRendererQueueItemUI) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UGamedevHelperRendererQueueItem>, QueueItem)
SLATE_END_ARGS()
	
void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
private:
	TWeakObjectPtr<UGamedevHelperRendererQueueItem> QueueItem;
};