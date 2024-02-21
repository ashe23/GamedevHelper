// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GdhStructs.generated.h"

USTRUCT(BlueprintType)
struct GDHCORE_API FGdhAffix
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct GDHCORE_API FGdhAssetAffixRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh", meta=(ShowTreeView, AllowAbstract))
	TSoftClassPtr<UObject> AssetClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct GDHCORE_API FGdhActorAffixRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh", meta=(ShowTreeView, AllowAbstract))
	TSoftClassPtr<UObject> ActorClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Prefix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Suffix;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gdh")
	FString Folder;
};
