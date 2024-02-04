// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"

class SGdhAssetNamingToolListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhAssetNamingToolListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhAssetNamingToolListItem) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhAssetNamingToolListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGdhAssetNamingToolListItem> ListItem;
};
