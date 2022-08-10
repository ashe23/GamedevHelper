// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGamedevHelperRenderingManagerSettings;
class UGamedevHelperRenderingManagerQueueSettings;
class UGamedevHelperRenderingManagerQueueItem;

class SGamedevHelperRenderingManagerUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRenderingManagerUI) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
protected:
	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;

	void ListUpdateData();
	void ListRefresh() const;

	FReply OnBtnRefreshClicked();
	bool IsBtnRefreshEnabled() const;
	
private:
	UGamedevHelperRenderingManagerSettings* RenderingManagerSettings = nullptr;
	UGamedevHelperRenderingManagerQueueSettings* RenderingManagerQueueSettings = nullptr;
	
	TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>> Queue;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>> QueueList;

	bool bCanStartRender = false;
};


