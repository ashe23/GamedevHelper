// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGamedevHelperRendererSettings;
class UGamedevHelperRendererQueueItem;

class SGamedevHelperRendererUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRendererUI) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	UGamedevHelperRendererSettings* RendererSettings = nullptr;
	TArray<TWeakObjectPtr<UGamedevHelperRendererQueueItem>> Queue;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperRendererQueueItem>>> QueueList;
	bool bCanStartRender = false;
	bool bRequireReRender = false;

	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGamedevHelperRendererQueueItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;

	FReply OnBtnRefreshClicked();
	FReply OnBtnRenderClicked();
	// FReply OnBtnCleanClicked();
	bool IsBtnRefreshEnabled() const;
	bool IsBtnRenderEnabled() const;

	void ListUpdateData();
	void ListRefresh() const;
	
};