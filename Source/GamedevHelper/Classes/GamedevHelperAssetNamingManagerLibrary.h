// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/AssetNamingManager/GamedevHelperAssetNamingManagerSettings.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetNamingManagerLibrary.generated.h"

UCLASS()
class UGamedevHelperAssetNamingManagerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static void FixName(const FAssetData& Asset, const UGamedevHelperAssetNamingManagerSettings& Settings);
	
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static void FixNames(const TArray<FAssetData>& Assets, const UGamedevHelperAssetNamingManagerSettings& Settings);
	
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString Normalize(const FString& OriginalString);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString Tokenize(const FString& OriginalString);

	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertNamingCase(const EGamedevHelperNamingCase NamingCase);
};
