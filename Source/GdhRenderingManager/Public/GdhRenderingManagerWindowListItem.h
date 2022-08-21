// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhRenderingManagerWindowListItem.generated.h"

UCLASS(Transient)
class GDHRENDERINGMANAGER_API UGdhRenderingManagerListItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class ULevelSequence* LevelSequence;

	UPROPERTY()
	class UMoviePipelineQueue* MoviePipelineQueue;
};

class GDHRENDERINGMANAGER_API SGdhRenderingManagerListItem : public SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderingManagerListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerListItem){}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhRenderingManagerListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGdhRenderingManagerListItem> ListItem;
};


