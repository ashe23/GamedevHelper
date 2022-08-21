// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE(FGdhDelegateRenderRequested);

class UGdhRenderingSettings;
class UGdhRenderingQueueSettings;
class UGdhRenderingManagerListItem;

class GDHRENDERINGMANAGER_API SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
private:
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGdhRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;

	TSharedPtr<SHeaderRow> GetHeaderRow() const;
	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
	static bool IsMovieRenderWorking();

	FReply OnBtnRenderClicked() const;
	FReply OnBtnRefreshClicked();

	bool IsBtnRenderEnabled() const;
	bool IsBtnRefreshEnabled() const;

	void ListUpdate();
	
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhRenderingQueueSettings* RenderingQueueSettings = nullptr;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> ListItems;
	TSharedPtr<SWidgetSwitcher> ListJobWidgetSwitcher;
	const int32 WidgetSwitcherNoJobsIndex = 0;
	const int32 WidgetSwitcherListIndex = 1;
public:
	FGdhDelegateRenderRequested GdhDelegateRenderRequested;
};