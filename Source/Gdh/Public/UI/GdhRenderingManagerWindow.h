// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderingSettings;
class UGdhMovieRenderSettings;
class UGdhRenderingQueueSettings;
class UGdhSubsystem;
class UGdhRenderingManagerListItem;

class SGdhRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
private:
	int32 GetActiveWidgetIndex() const;
	FText GetConsoleBoxText() const;
	FText GetJobStats() const;
	EVisibility GetConsoleBoxVisibility() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGdhRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;
	TSharedPtr<SHeaderRow> GetHeaderRow() const;

	void ListUpdate();

	FReply OnBtnRefreshClick();
	FReply OnBtnRenderClick();

	bool IsBtnRenderEnabled() const;
	
	UGdhRenderingSettings* RenderingSettings = nullptr;
	UGdhMovieRenderSettings* MovieRenderSettings = nullptr;
	UGdhRenderingQueueSettings* QueueSettings = nullptr;
	UGdhSubsystem* GdhSubsystem = nullptr;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhRenderingManagerListItem>> ListItems;
	const int32 WidgetIndexRenderingManagerWorking = 0;
	const int32 WidgetIndexRenderingManagerUI = 1;
	FString ErrorMsg;
	FString JobStats;
};