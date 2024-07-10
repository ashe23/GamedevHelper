// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhActorNamingToolListItem.generated.h"

UCLASS(Transient)
class UGdhActorNamingToolListItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Prefix;

	UPROPERTY()
	FString Suffix;

	UPROPERTY()
	FString OldName;

	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString FolderName;

	UPROPERTY()
	AActor* Actor;
};

class SGdhActorNamingToolListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhActorNamingToolListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhActorNamingToolListItem) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhActorNamingToolListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGdhActorNamingToolListItem> ListItem;
};