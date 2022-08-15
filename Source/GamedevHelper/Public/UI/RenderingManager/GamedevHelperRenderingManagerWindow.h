// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h"

class UGamedevHelperRenderingSettings;
// class UGamedevHelperRenderingManagerSettings;
class UGamedevHelperRenderingManagerQueueSettings;
class UGamedevHelperRenderingManagerListItem;

class SGamedevHelperRenderingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGamedevHelperRenderingManagerWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
protected:
	FText GetConsoleBoxText() const;
	EVisibility GetConsoleBoxVisibility() const;
	TSharedRef<ITableRow> OnGenerateRow(
		TWeakObjectPtr<UGamedevHelperRenderingManagerListItem> InItem,
		const TSharedRef<STableViewBase>& OwnerTable
	) const;

	void ListUpdateData();
	void ListRefresh() const;
	bool IsMovieRenderWorking() const;

	FReply OnBtnRefreshClicked();
	FReply OnBtnRenderClicked() const;
	FReply OnBtnCleanOutputDirClicked() const;
	FReply OnBtnOpenOutputDirClicked() const;
	bool IsBtnRefreshEnabled() const;
	bool IsBtnRenderEnabled() const;
	bool IsBtnCleanOutputDirEnabled() const;
	bool IsBtnOpenOutputDirEnabled() const;
	
private:
	UGamedevHelperRenderingSettings* RenderingSettings = nullptr;
	UGamedevHelperRenderingManagerQueueSettings* RenderingManagerQueueSettings = nullptr;
	
	TArray<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>> ListItems;
	TSharedPtr<SListView<TWeakObjectPtr<UGamedevHelperRenderingManagerListItem>>> ListView;
};


