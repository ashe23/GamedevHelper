// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhAssetNamingConvention.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Asset Naming Convention")
class UGdhAssetNamingConvention final : public UDeveloperSettings
{
public:
	GENERATED_BODY()

	UGdhAssetNamingConvention();

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;
	FGdhSettingsChanged& OnSettingsChanged();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention")
	EGdhNamingCase NamingCase = EGdhNamingCase::PascalSnakeCase;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", meta=(ToolTip="List of prefixes that should be removed from asset name"))
	TArray<FString> OldPrefixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", meta=(ToolTip="List of suffixes that should be removed from asset name"))
	TArray<FString> OldSuffixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", meta=(ToolTip="Blueprint specific prefixes"))
	TMap<EGdhBlueprintType, FGdhAssetNamingInfo> BlueprintPrefixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", DisplayName="Asset Namings", meta=(ToolTip="Asset class and name format mappings"))
	TMap<UClass*, FGdhAssetNamingInfo> Mappings;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", meta=(ToolTip="Assets that must not be renamed. Ignore List"))
	TArray<TSoftObjectPtr<UObject>> AssetsIgnore;

private:
	FGdhSettingsChanged DelegateSettingsChanged;
};
