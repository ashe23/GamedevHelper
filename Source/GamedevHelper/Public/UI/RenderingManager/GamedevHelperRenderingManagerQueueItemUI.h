// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UGamedevHelperRenderingManagerQueueItem;

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


