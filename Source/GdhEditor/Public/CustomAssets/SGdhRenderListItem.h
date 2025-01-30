// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhRenderListItem.generated.h"

UCLASS(Transient)
class UGdhRenderListItem : public UObject
{
public:

	GENERATED_BODY()

	FString Name;
	FString FrameStart;
	FString FrameEnd;
	FString FrameRate;
	FString Duration;
};

class SGdhRenderListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderListItem>>
{
public:

	SLATE_BEGIN_ARGS(SGdhRenderListItem) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UGdhRenderListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:

	TWeakObjectPtr<UGdhRenderListItem> ListItem;
};
