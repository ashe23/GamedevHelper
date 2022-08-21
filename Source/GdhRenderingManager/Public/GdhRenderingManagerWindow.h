// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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
	bool IsMovieRenderWorking() const;

	FReply OnBtnRenderClicked();

	void ListUpdate();
	
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhRenderingQueueSettings* RenderingQueueSettings = nullptr;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> ListItems;
	TSharedPtr<SWidgetSwitcher> ListJobWidgetSwitcher;
};