// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetNamingManagerSettings.generated.h"

UCLASS(Transient)
class UGamedevHelperAssetNamingManagerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingManagerSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ContentDir))
	FDirectoryPath ScanPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	EGamedevHelperNamingCase NamingCase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	bool bIgnoreNamingCaseOnPrefixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	bool bIgnoreNamingCaseOnSuffixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<EGamedevHelperBlueprintType, FGamedevHelperAssetNameSettings> BlueprintTypesNaming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> AssetsNaming;

private:
	void SetDefaultSettings();
};
