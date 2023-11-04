// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhAssetScanSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings)
class UGdhAssetScanSettings : public UObject
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="ScanSettings", meta=(ContentDir))
	FDirectoryPath ScanPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="ScanSettings")
	bool bScanRecursive = false;
};
