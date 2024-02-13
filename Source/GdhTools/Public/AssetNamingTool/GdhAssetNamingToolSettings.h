// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.h"
#include "GdhAssetNamingToolSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Settings")
class UGdhAssetNamingToolSettings : public UObject
{
public:
	GENERATED_BODY()

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Data Table Asset with Asset Class => (Prefix, Suffix) Mappings. To create new mappings, just create new data table with row struct "GdhAssetNameAffixRow" selected. Or just duplicate existing one.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", meta=(RequiredAssetDataTags="RowStructure=GdhAssetNameAffixRow"))
	TSoftObjectPtr<UDataTable> Mappings;
	
	// Naming case for asset name only. This does not apply to prefixes or suffixes. Pascal_Snake_Case is recommended.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhNamingCase AssetNamingCase = EGdhNamingCase::PascalSnakeCase;

	// Naming case for prefixes. UPPERCASE is recommended.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhNamingCase PrefixNamingCase = EGdhNamingCase::UpperCase;

	// Naming case for suffixes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhNamingCase SuffixNamingCase = EGdhNamingCase::UpperCase;

	// What delimiter type to use when gluing prefixes and suffixes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhAssetNameDelimiter Delimiter = EGdhAssetNameDelimiter::Underscore;
};