// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperAssetNamingManagerSettings.generated.h"

DECLARE_DELEGATE(FAssetNamingManagerSettingsChangeDelegate);

UCLASS(Transient)
class UGamedevHelperAssetNamingManagerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingManagerSettings();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ContentDir))
	FDirectoryPath ScanPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ToolTip = "Scan folder recursively"))
	bool bScanRecursive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ToolTip = "If enabled will show assets whose classes are missing in naming convention list"))
	bool bShowMissingTypes;
	
	FAssetNamingManagerSettingsChangeDelegate OnSettingsChangeDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ToolTip = "List of prefixes that will be removed from asset name, when renaming"))
	TArray<FString> OldPrefixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ToolTip = "List of suffixes that will be removed from asset name, when renaming"))
	TArray<FString> OldSuffixes;

private:
	void SetDefaultSettings();
};
