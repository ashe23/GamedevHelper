// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.generated.h"


UCLASS(Transient)
class UGdhManagerAssetNamingItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FAssetData AssetData;

	UPROPERTY()
	FString OldName;

	UPROPERTY()
	FString NewName;

	UPROPERTY()
	FString Note;
};
