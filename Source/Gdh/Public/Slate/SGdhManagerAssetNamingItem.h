// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UGdhManagerAssetNamingItem;

class SGdhManagerAssetNamingItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhManagerAssetNamingItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhManagerAssetNamingItem) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhManagerAssetNamingItem>, RowItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGdhManagerAssetNamingItem> RowItem;
};
