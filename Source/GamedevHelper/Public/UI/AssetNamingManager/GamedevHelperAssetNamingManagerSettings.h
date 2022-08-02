// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperAssetNamingManagerSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings)
class UGamedevHelperAssetNamingManagerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingManagerSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		SaveConfig();
	}
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="AssetNamingManagerSettings", meta = (ContentDir))
	FDirectoryPath ScanPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="AssetNamingManagerSettings", meta = (ToolTip = "Scan folder recursively"))
	bool bScanRecursive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="AssetNamingManagerSettings", meta = (ToolTip = "If enabled will show assets whose classes are missing in naming convention list"))
	bool bShowMissingTypes = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="AssetNamingManagerSettings", meta = (ToolTip = "List of prefixes that will be removed from asset name, when renaming"))
	TArray<FString> OldPrefixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category="AssetNamingManagerSettings", meta = (ToolTip = "List of suffixes that will be removed from asset name, when renaming"))
	TArray<FString> OldSuffixes;
};
