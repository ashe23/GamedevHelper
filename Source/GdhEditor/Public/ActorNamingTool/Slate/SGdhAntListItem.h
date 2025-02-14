// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhAntListItem.generated.h"

UCLASS(Transient)
class UGdhAntListItem : public UObject
{
	GENERATED_BODY()

public:

	FString Prefix;
	FString Suffix;
	FString OldName;
	FString NewName;
	FString FolderName;

	UPROPERTY()
	AActor* Actor;

	const FSlateBrush* ActorIcon;
};

class SGdhAntListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhAntListItem>>
{
public:

	SLATE_BEGIN_ARGS(SGdhAntListItem) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UGdhAntListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:

	TWeakObjectPtr<UGdhAntListItem> ListItem;
};
