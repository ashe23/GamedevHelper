﻿// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GdhStructs.generated.h"

USTRUCT(BlueprintType)
struct GDHCORE_API FGdhAssetNameAffix
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct GDHCORE_API FGdhAssetNameAffixRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh", meta=(ShowTreeView, AllowAbstract))
	TSoftClassPtr<UObject> AssetClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;
};
