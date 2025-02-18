// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhBetRenderListItem.generated.h"

class ULevelSequence;

UCLASS(Transient)
class UGdhBetRenderListItem : public UObject
{
public:

	GENERATED_BODY()

	FString Name;
	FString FrameStart;
	FString FrameEnd;
	FString FrameRate;
	FString DurationFrames;
	FString DurationHuman;
	FString HasTrackSlomo;
	FString EncodePresetName;
	FString EncodeCmdPreview;

	UPROPERTY()
	const ULevelSequence* Sequence = nullptr;
};

class SGdhBetRenderListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhBetRenderListItem>>
{
public:

	SLATE_BEGIN_ARGS(SGdhBetRenderListItem) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UGdhBetRenderListItem>, ListItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:

	TWeakObjectPtr<UGdhBetRenderListItem> ListItem;
};
