// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/GdhAssetNamingConventionSettings.h"
#include "Widgets/SCompoundWidget.h"
#include "GdhAssetNamingManagerWindow.generated.h"

DECLARE_DELEGATE(FGdhAssetNamingManagerScanSettingsOnChangeDelegate);

class UGdhAssetNamingManagerListItem;
struct FGdhRenamePreview;

UCLASS(Config=EditorPerProjectUserSettings)
class UGdhAssetNamingManagerScanSettings : public UObject
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		SaveConfig();

		if (OnChangeDelegate.IsBound())
		{
			OnChangeDelegate.Execute();
		}
	}
#endif

	FGdhAssetNamingManagerScanSettingsOnChangeDelegate& OnChange()
	{
		return OnChangeDelegate;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="Settings", meta = (ContentDir))
	FDirectoryPath ScanPath{TEXT("/Game")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="Settings", meta = (ToolTip = "Scan folder recursively"))
	bool bScanRecursive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="Settings", meta = (ToolTip = "If enabled will show assets whose classes are missing in naming convention list"))
	bool bShowMissingTypes = false;
private:
	FGdhAssetNamingManagerScanSettingsOnChangeDelegate OnChangeDelegate;
};

class SGdhAssetNamingManagerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGdhAssetNamingManagerWindow){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	void ListUpdate();
	void ListSort();
	void OnListSort(EColumnSortPriority::Type SortPriority, const FName& Name, EColumnSortMode::Type SortMode);
	void UpdateRenamePreviews(const TArray<FAssetData>& Assets);
	FGdhAssetAppendix GetNameFormatByAssetData(const FAssetData& Asset) const;
	FGdhAssetAppendix GetNameFormatByClass(const UClass* SearchClass) const;
	TSharedRef<ITableRow> OnGenerateRow(TWeakObjectPtr<UGdhAssetNamingManagerListItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) const;

	
	TWeakObjectPtr<UGdhAssetNamingConventionSettings> ConventionSettings;
	TWeakObjectPtr<UGdhAssetNamingManagerScanSettings> ScanSettings;
	TSharedPtr<SListView<TWeakObjectPtr<UGdhAssetNamingManagerListItem>>> ListView;
	TArray<TWeakObjectPtr<UGdhAssetNamingManagerListItem>> ListItems;
	TArray<FGdhRenamePreview> RenamePreviews;
	TEnumAsByte<EColumnSortMode::Type> CurrentSortMode = EColumnSortMode::Ascending;
	FName SortColumn = TEXT("AssetClass");
	bool bRenameBtnActive = false;
};