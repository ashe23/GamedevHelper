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

	FGamedevHelperAssetNameFormat GetAssetNameFormat(const FAssetData& Asset) const;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingConvention")
	TMap<EGamedevHelperBlueprintType, FString> BlueprintTypePrefixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetNamingConvention")
	TMap<UClass*, FGamedevHelperAssetNameFormat> Namings;

	FAssetNamingConventionChangeDelegate OnConventionPropertyChangeDelegate;

private:
	FGamedevHelperAssetNameFormat GetNaming(const FAssetData& Asset) const;
	FGamedevHelperAssetNameFormat GetNamingByClass(const UClass* SearchClass) const;
	
	void SetDefaultSettings();
	void SetBlueprintNamings();
	void SetAnimationNamings();
	void SetAINamings();
	void SetMaterialNamings();
	void SetTextureNamings();
	void SetFXNamings();
	void SetFoliageNamings();
	void SetPaper2DNamings();
	void SetSlateNamings();
	void SetSoundNamings();
	void SetMiscNamings();
	
};