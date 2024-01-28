// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhAssetNamingSettings.generated.h"

UCLASS(BlueprintType)
class GDH_API UGdhAssetNamingSettings : public UObject
{
	GENERATED_BODY()

public:
	UGdhAssetNamingSettings();

	UPROPERTY(EditAnywhere)
	EGdhNamingCase NamingCase;

	UPROPERTY(EditAnywhere)
	TMap<TSoftClassPtr<UObject>, FGdhAssetNamingInfo> Mappings;
};
