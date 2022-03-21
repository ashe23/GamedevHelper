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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	bool bScanRecursive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	bool bShowMissingTypes;
	
	FAssetNamingManagerSettingsChangeDelegate OnSettingsChangeDelegate;

private:
	void SetDefaultSettings();
};
