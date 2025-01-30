// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UGdhRenderList;
class UGdhRenderListItem;

class SGdhRenderList final : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGdhRenderList) {}
	SLATE_ARGUMENT(UGdhRenderList*, RenderList)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	void ListUpdateData();
	void ListUpdateView();

	FReply OnDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);
	bool CanDragDropTarget(TSharedPtr<FDragDropOperation> InOperation);

	TSharedRef<SHeaderRow> GetHeaderRow();
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhRenderListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TArray<TWeakObjectPtr<UGdhRenderListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhRenderListItem>>> ListView;

	UGdhRenderList* RenderList = nullptr;
};
