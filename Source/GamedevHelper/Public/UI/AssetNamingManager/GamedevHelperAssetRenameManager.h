// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetRenameManager.generated.h"

class UGamedevHelperAssetNamingConvention;

UCLASS()
class UGamedevHelperRenameManager : public UObject
{
	GENERATED_BODY()
public:
	void RenameAssets(TArray<FAssetData>& Assets, const UGamedevHelperAssetNamingConvention* NamingConvention);
private:
};
