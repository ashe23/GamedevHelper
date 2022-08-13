// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h"

class UGamedevHelperRenderingSettings;
// class UGamedevHelperRenderingManagerSettings;
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
	FReply OnBtnRenderClicked();
	FReply OnBtnOpenOutputDirClicked();
	bool IsBtnRefreshEnabled() const;
	bool IsBtnRenderEnabled() const;
	bool IsBtnOpenOutputDirEnabled() const;

	void ExportToJson() const;
	
private:
	UGamedevHelperRenderingSettings* RenderingSettings = nullptr;
	UGamedevHelperRenderingManagerQueueSettings* RenderingManagerQueueSettings = nullptr;
	
	TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>> Queue;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperRenderingManagerQueueItem>>> QueueList;
};


