// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGdhAssetNamingToolListItem.generated.h"

class UGdhAssetNamingToolSettings;

UCLASS(Transient)
class UGdhAssetNamingToolListItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bDirty = false;

	UPROPERTY()
	bool bHasErrors = false;

	UPROPERTY()
	bool bEditMode = false;

	UPROPERTY()
	FAssetData AssetData;

	UPROPERTY()
	FString Prefix;

	UPROPERTY()
	FString Suffix;

	UPROPERTY()
	FString OldName;

	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString Note;
};

DECLARE_DELEGATE_TwoParams(FGdhDelegateRowAssetNameChanged, const TWeakObjectPtr<UGdhAssetNamingToolListItem>& Item, const FString& InputText);

class SGdhAssetNamingToolListItem final : public SMultiColumnTableRow<TWeakObjectPtr<UGdhAssetNamingToolListItem>>
{
public:
	SLATE_BEGIN_ARGS(SGdhAssetNamingToolListItem) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UGdhAssetNamingToolListItem>, ListItem)
		SLATE_EVENT(FGdhDelegateRowAssetNameChanged, OnAssetNameChange)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;
	
private:
	void OnItemNameChanged(const FText& Text) const;

	TSharedPtr<STextBlock> NoteBlock;
	TSharedPtr<SImage> StatusIcon;
	TSharedPtr<STextBlock> NamePreview;
	TWeakObjectPtr<UGdhAssetNamingToolListItem> ListItem;
	FGdhDelegateRowAssetNameChanged DelegateRowAssetNameChanged;
};
