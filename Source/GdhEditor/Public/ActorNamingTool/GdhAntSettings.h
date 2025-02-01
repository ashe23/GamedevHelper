// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.h"
#include "GdhAntSettings.generated.h"

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhAntSettings : public UObject
{
public:

	GENERATED_BODY()

	UGdhAntSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Data Table asset containing settings for renaming and organizing actors in World Outlinear
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category = "Settings", meta = (RequiredAssetDataTags = "RowStructure=GdhActorAffixRow"))
	TSoftObjectPtr<UDataTable> Mappings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category = "Settings")
	EGdhNamingCase ActorNamingCase = EGdhNamingCase::PascalSnakeCase;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category = "Settings")
	EGdhNamingCase PrefixNamingCase = EGdhNamingCase::UpperCase;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category = "Settings")
	EGdhNamingCase SuffixNamingCase = EGdhNamingCase::None;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category = "Settings")
	EGdhNamingCase FolderNamingCase = EGdhNamingCase::PascalSnakeCase;
};
