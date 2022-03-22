// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetNamingConvention.generated.h"

DECLARE_DELEGATE(FAssetNamingConventionChangeDelegate);

UCLASS(config=EditorPerProjectUserSettings)
class UGamedevHelperAssetNamingConvention : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperAssetNamingConvention();

	void GetAssetClasses(TArray<UClass*>& Classes);
	FGamedevHelperAssetNameFormat GetAssetNameFormat(const FAssetData& Asset) const;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingConvention")
	// TMap<EGamedevHelperBlueprintType, FGamedevHelperAssetNameFormat> BlueprintTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingConvention")
	TMap<EGamedevHelperAssetType, FGamedevHelperAssetNamings> Namings;

	FAssetNamingConventionChangeDelegate OnConventionPropertyChangeDelegate;

private:
	void SetDefaultSettings();
	
	static FGamedevHelperAssetNamings GetMaterialNamings();
	static FGamedevHelperAssetNamings GetTextureNamings();
	static FGamedevHelperAssetNamings GetAnimationNamings();
	static FGamedevHelperAssetNamings GetFXNamings();
	static FGamedevHelperAssetNamings GetAINamings();
	static FGamedevHelperAssetNamings GetFoliageNamings();
	static FGamedevHelperAssetNamings GetSlateNamings();
	static FGamedevHelperAssetNamings GetPaper2DNamings();
	static FGamedevHelperAssetNamings GetSoundNamings();
	static FGamedevHelperAssetNamings GetMiscNamings();
	static FGamedevHelperAssetNamings GetBlueprintNamings();
};