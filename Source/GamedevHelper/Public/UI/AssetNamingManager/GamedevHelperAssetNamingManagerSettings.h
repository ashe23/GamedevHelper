// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetNamingManagerSettings.generated.h"

DECLARE_DELEGATE(FAssetNamingManagerSettingsChangeDelegate);

// todo:ashe23 transient must be changed to config, because we will be able to import/export settings
UCLASS(Transient)
class UGamedevHelperAssetNamingManagerSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingManagerSettings();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	const FGamedevHelperAssetNameSettings* FindNamingByClass(const UClass* AssetClass) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", meta = (ContentDir))
	FDirectoryPath ScanPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	bool bScanRecursive;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	// EGamedevHelperNamingCase NamingCase;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	// bool bIgnoreNamingCaseOnPrefixes;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	// bool bIgnoreNamingCaseOnSuffixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<EGamedevHelperBlueprintType, FGamedevHelperAssetNameSettings> BlueprintTypesNaming;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> MaterialTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> TextureTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> AnimationTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", DisplayName = "FX Type Assets")
	TMap<UClass*, FGamedevHelperAssetNameSettings> FXTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", DisplayName = "AI Type Assets")
	TMap<UClass*, FGamedevHelperAssetNameSettings> AITypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> FoliageTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> SoundTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> SlateTypeAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings", DisplayName = "Paper2D Type Assets")
	TMap<UClass*, FGamedevHelperAssetNameSettings> Paper2DTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> BlueprintTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> EditorScriptingUtilitiesTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> MiscTypeAssets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingManagerSettings")
	TMap<UClass*, FGamedevHelperAssetNameSettings> CustomTypeAssets;
	
	// Called when settings changed
	FAssetNamingManagerSettingsChangeDelegate OnSettingsChangeDelegate;

private:
	void SetDefaultSettings();
};
