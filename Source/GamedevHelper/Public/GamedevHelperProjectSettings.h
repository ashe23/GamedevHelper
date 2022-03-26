// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperProjectSettings.generated.h"

UCLASS(Config=EditorUserSettings)
class UGamedevHelperAssetNamingConventionSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingConventionSettings();
	FGamedevHelperAssetNameFormat GetNameFormatByAssetData(const FAssetData& Asset) const;
	FGamedevHelperAssetNameFormat GetNameFormatByClass(const UClass* SearchClass) const;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", DisplayName = "BlueprintPrefixes", meta = (ToolTip = "Blueprint prefixes based on blueprint type"))
	TMap<EGamedevHelperBlueprintType, FString> BlueprintTypePrefixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", DisplayName = "Asset Namings", meta = (ToolTip = "Asset class and name format mappings"))
	TMap<UClass*, FGamedevHelperAssetNameFormat> Mappings;
};

UCLASS(Config=EditorUserSettings)
class UGamedevHelperWorldOutlinerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperWorldOutlinerSettings();
	FName GetFolderNameByActor(const AActor* Actor);
	
	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Move To Unsorted",  meta = (ToolTip = "Move actors whos class is not specified, to 'Unsorted' folder"))
	bool bMoveToUnsorted = true;

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "Unsorted folder name", meta = (ToolTip = "Unsorted folder name. Default 'Unsorted'", EditCondition = "bMoveToUnsorted"))
	FString UnsortedFolderName = TEXT("Unsorted");

	UPROPERTY(EditAnywhere, Config, Category="World Outliner Settings", DisplayName = "World outliner mappings", meta = (ToolTip = "Actor class and folder name mappings"))
	TMap<UClass*, FName> Mappings;
};