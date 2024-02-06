// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhAssetNamingToolSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Settings")
class UGdhAssetNamingToolSettings : public UObject
{
public:
	GENERATED_BODY()

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Scan assets in given path
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="ScanSettings", meta=(ContentDir))
	FDirectoryPath ScanPath = FDirectoryPath{TEXT("/Game")};

	// Scan directory recursive
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="ScanSettings")
	bool bScanRecursive = false;

	// Naming case for asset name only. This does not apply to prefixes or suffixes. Pascal_Snake_Case is recommended.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings")
	EGdhNamingCase AssetNamingCase = EGdhNamingCase::PascalSnakeCase;

	// Naming case for prefixes. UPPERCASE is recommended.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings")
	EGdhNamingCase PrefixNamingCase = EGdhNamingCase::UpperCase;

	// Naming case for suffixes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings")
	EGdhNamingCase SuffixNamingCase = EGdhNamingCase::UpperCase;

	// What delimiter type to use when gluing prefixes and suffixes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings")
	EGdhDelimiter Delimiter = EGdhDelimiter::Underscore;

	// Data Table Asset with Asset Class => (Prefix, Suffix) Mappings. To create new mappings, just create new data table with row struct "GdhAssetNameFormatRow" selected. Or just duplicate existing one.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings", meta=(RequiredAssetDataTags="RowStructure=GdhAssetNameFormatRow"))
	TSoftObjectPtr<UDataTable> Mappings;

	// In the event that an asset with the same name already exists in the specified location, enabling this parameter will add a numerized suffix to the asset name for differentiation. Otherwise those assets will not be renamed.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings")
	bool bNumerizeAssetOnConflict = false;

	// Specifies the number of leading zeros to append to the number. For example: Name_1, Name_01, Name_001, etc.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings", meta=(EditCondition="bNumerizeAssetOnConflict", EditConditionHides, UIMin="0", ClampMin="0", UIMax="10", ClampMax="10"))
	int32 NumerizeZeroPadding = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings")
	bool bCustomReplace = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings", meta=(EditCondition="bCustomReplace", EditConditionHides))
	bool bRegexSearch = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings", meta=(EditCondition="bCustomReplace && bRegexSearch", EditConditionHides))
	FString RegexSearch;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings", meta=(EditCondition="bCustomReplace && !bRegexSearch", EditConditionHides))
	bool bReplaceCaseSensitive = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingSettings", meta=(EditCondition="bCustomReplace && !bRegexSearch", EditConditionHides))
	TMap<FString, FString> ReplaceMappings;
};
