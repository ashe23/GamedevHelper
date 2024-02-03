// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhNamingPolicyAsset.generated.h"

UCLASS(BlueprintType, DisplayName="Gdh Naming Policy Asset")
class GDH_API UGdhNamingPolicyAsset : public UObject
{
	GENERATED_BODY()

public:
	UGdhNamingPolicyAsset();

	// Default Asset naming case
	UPROPERTY(EditAnywhere)
	EGdhNamingCase NamingCase = EGdhNamingCase::PascalSnakeCase;

	// Naming formats based on blueprint type
	UPROPERTY(EditAnywhere)
	TMap<EGdhBlueprintType, FGdhAssetNameFormat> BlueprintTypes;

	// Data Table Asset with Asset Class Mappings
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> Mappings;

	// In the event that an asset with the same name already exists in the specified location, enabling this parameter will add a numerized suffix to the asset name for differentiation. Otherwise those assets will not be renamed.
	UPROPERTY(EditAnywhere)
	bool bNumerizeAssetOnConflict = false;

	// Specifies the number of leading zeros to append to the number. For example: Name_1, Name_01, Name_001, etc.
	UPROPERTY(EditAnywhere, meta=(EditCondition="bNumerizeAssetOnConflict", UIMin="0", ClampMin="0", UIMax="10", ClampMax="10"))
	int32 NumerizeZeroPadding = 0;
};
