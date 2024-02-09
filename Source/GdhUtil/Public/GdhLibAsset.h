// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibAsset.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhAssetLibrary"))
class GDHUTIL_API UGdhLibAsset : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns all assets in project (particularly Content folder)
	 * @param Assets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetsAll(TArray<FAssetData>& Assets);

	/**
	 * @brief Returns all primary and derived from primary assets in project. See AssetManager Settings for more info.
	 * @param Assets TArray<FAssetData>
	 * @param bShowSlowTask bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetsPrimary(TArray<FAssetData>& Assets, const bool bShowSlowTask = true);

	/**
	 * @brief Returns all primary assets class names
	 * @param ClassNames TSet<FName>
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetClassNamesPrimary(TSet<FName>& ClassNames);

	/**
	 * @brief Returns asset exact class name, if its blueprint it will return generated class name
	 * @param InAsset FAssetData
	 * @return FName 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static FName GetAssetExactClassName(const FAssetData& InAsset);

	/**
	 * @brief Returns given asset size on disk in bytes
	 * @param InAsset FAssetData
	 * @return int64
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static int64 GetAssetSize(const FAssetData& InAsset);

	/**
	 * @brief Returns total size of given assets in bytes
	 * @param InAssets TArray<FAssetData>
	 * @return int64
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static int64 GetAssetsTotalSize(const TArray<FAssetData>& InAssets);

	/**
	 * @brief Checks if given asset is blueprint or not
	 * @param InAsset FAssetData
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static bool AssetIsBlueprint(const FAssetData& InAsset);

	/**
	 * @brief Checks if given asset has external referencers outside Content folder or not
	 * @param InAsset FAssetData
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static bool AssetIsExtReferenced(const FAssetData& InAsset);

	/**
	 * @brief Checks if given asset has circular dependencies or not
	 * @param InAsset FAssetData
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static bool AssetIsCircular(const FAssetData& InAsset);

	/**
	 * @brief Returns all redirectors in project
	 * @param Redirectors TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetProjectRedirectors(TArray<FAssetData>& Redirectors);

	/**
	 * @brief Checks if project contains any redirector asset
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static bool ProjectHasRedirectors();

	/**
	 * @brief Fixup given redirector assets
	 * @param Redirectors TArray<FAssetData>
	 * @param bShowSlowTask bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void FixProjectRedirectors(const TArray<FAssetData>& Redirectors, const bool bShowSlowTask = true);
};
