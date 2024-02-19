// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.h"
#include "GdhStructs.h"
#include "GdhAssetNamingToolSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Settings")
class UGdhAssetNamingToolSettings : public UObject
{
public:
	GENERATED_BODY()

	UGdhAssetNamingToolSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Data Table Asset with Asset Class => (Prefix, Suffix) Mappings. To create new mappings, just create new data table with row struct "GdhAssetNameAffixRow" selected.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", DisplayName="Asset Affix Mappings", meta=(RequiredAssetDataTags="RowStructure=GdhAssetNameAffixRow"))
	TSoftObjectPtr<UDataTable> Mappings;

	// Blueprint type affix mappings
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", meta=(ForceInlineRow, TitleProperty="Prefix"))
	TMap<EGdhBlueprintType, FGdhAssetNameAffix> BlueprintTypes;

	// Naming case for asset name only. This does not apply to prefixes or suffixes. Pascal_Snake_Case is recommended.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhNamingCase AssetNamingCase = EGdhNamingCase::PascalSnakeCase;

	// Naming case for prefixes. UPPERCASE is recommended.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhNamingCase PrefixNamingCase = EGdhNamingCase::UpperCase;

	// Naming case for suffixes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention")
	EGdhNamingCase SuffixNamingCase = EGdhNamingCase::UpperCase;
};
