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

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", meta=(ToolTip="Keywords that must be never changed"))
	TArray<FString> ReservedKeywords;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention")
	EGdhNamingCase NamingCase = EGdhNamingCase::PascalSnakeCase;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention")
	bool bApplyNamingCaseOnPrefixAndSuffix = false;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention")
	TArray<FString> OldPrefixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention")
	TArray<FString> OldSuffixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", meta=(ToolTip="Blueprint specific prefixes"))
	TMap<EGdhBlueprintType, FGdhAssetNamingInfo> BlueprintPrefixes;

	UPROPERTY(EditAnywhere, Config, Category="Asset Naming Convention", DisplayName="Asset Namings", meta=(ToolTip="Asset class and name format mappings"))
	TMap<UClass*, FGdhAssetNamingInfo> Mappings;

private:
	FGdhSettingsChanged DelegateSettingsChanged;
};
