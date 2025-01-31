// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhVideoEncoderToolListItem.generated.h"

UCLASS(Transient)
class UGdhVideoEncoderToolListItem : public UObject
{
public:

	GENERATED_BODY()

	FString NameQueue;
	FString NameSequence;
	FString FrameStart;
	FString FrameEnd;
	FString FrameRate;
	FString Duration;
};

class SGdhVideoEncoderToolListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhVideoEncoderToolListItem>>
{
public:

	SLATE_BEGIN_ARGS(SGdhVideoEncoderToolListItem) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UGdhVideoEncoderToolListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:

	TWeakObjectPtr<UGdhVideoEncoderToolListItem> ListItem;
};
