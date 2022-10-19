// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhRenderingManagerListItem.generated.h"

// class UMoviePipelineQueue;

UCLASS(Transient)
class UGdhRenderingManagerListItem : public UObject
{
	GENERATED_BODY()
public:
	FString Name;
	FString Duration;
	FString RenderedFrames;
	
	// TSoftObjectPtr<UWorld> Map;
	// TSoftObjectPtr<ULevelSequence> LevelSequence;
	// TSoftObjectPtr<UMoviePipelineQueue> MoviePipelineQueue;
};

class SGdhRenderingManagerListItem : public SMultiColumnTableRow<TWeakObjectPtr<UGdhRenderingManagerListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhRenderingManagerListItem) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhRenderingManagerListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	TWeakObjectPtr<UGdhRenderingManagerListItem> ListItem;
};