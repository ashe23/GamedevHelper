// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhPluginSettings.generated.h"

struct FGdhAffix;
enum class EGdhBlueprintType : uint8;

UCLASS(Config = EditorPerProjectUserSettings)
class GDHTOOLS_API UGdhPluginSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()

	UGdhPluginSettings();
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Data Table Asset with Asset Class => (Prefix, Suffix) Mappings. To create new mappings, just create new data table with row struct "GdhAssetAffixRow" selected.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", meta=(RequiredAssetDataTags="RowStructure=GdhAssetAffixRow"))
	TSoftObjectPtr<UDataTable> AssetAffixes;

	// Similar to AssetAffixes, but this contains mappings for actors in world outlinear
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", meta=(RequiredAssetDataTags="RowStructure=GdhActorAffixRow"))
	TSoftObjectPtr<UDataTable> ActorAffixes;

	// Affix mapping for blueprint types
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", meta=(ForceInlineRow, TitleProperty="Prefix"))
	TMap<EGdhBlueprintType, FGdhAffix> BlueprintTypeAffixes;
};
