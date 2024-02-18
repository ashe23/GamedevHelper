// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.h"
#include "GdhStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GdhLibAsset.generated.h"

struct FGdhAssetIndirectInfo;

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhAssetLibrary"))
class GDHUTIL_API UGdhLibAsset : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns all assets in project (Content folder)
	 * @param Assets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetsAll(TArray<FAssetData>& Assets);

	/**
	 * @brief Returns assets in given path
	 * @param Path FString
	 * @param bRecursive bool
	 * @param Assets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetByPath(const FString& Path, const bool bRecursive, TArray<FAssetData>& Assets);

	/**
	 * @brief Returns all primary and derived from primary assets in project. See AssetManager Settings for more info.
	 * @param Assets TArray<FAssetData>
	 * @param bShowSlowTask bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetsPrimary(TArray<FAssetData>& Assets, const bool bShowSlowTask = true);

	/**
	 * @brief Returns assets that used in source code or config files indirectly.
	 * @param Assets TArray<FAssetData> - Assets
	 * @param bShowSlowTask bool - Show slow task progress bar or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetsIndirect(TArray<FAssetData>& Assets, const bool bShowSlowTask = true);

	/**
	 * @brief Returns assets that contain unicode characters in their name
	 * @param Assets TArray<FAssetData>
	 * @param bShowSlowTask bool - Show slow task progress bar or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static void GetAssetsUnicode(TArray<FAssetData>& Assets, const bool bShowSlowTask = true);

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static FString GetAssetTagValue(const FAssetData& InAsset, const FName& Tag);

	/**
	 * @brief Returns asset name affix (Prefix + Suffix) for given asset and look up data table
	 * @param InAsset FAssetData
	 * @param Mappings UDataTable
	 * @param BlueprintTypes TMap<EGdhBlueprintType, FGdhAssetNameAffix>
	 * @return FGdhAssetNameAffix
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static FGdhAssetNameAffix GetAssetNameAffix(const FAssetData& InAsset, const UDataTable* Mappings, const TMap<EGdhBlueprintType, FGdhAssetNameAffix>& BlueprintTypes);

	/**
	 * @brief Returns corrected name of asset by naming convention
	 * @param Name FString Original asset name
	 * @param Affix FGdhAssetNameAffix
	 * @param AssetNamingCase EGdhNamingCase
	 * @param PrefixNamingCase EGdhNamingCase
	 * @param SuffixNamingCase EGdhNamingCase
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static FString GetAssetNameByConvention(const FString& Name, const FGdhAssetNameAffix& Affix, const EGdhNamingCase AssetNamingCase, const EGdhNamingCase PrefixNamingCase, const EGdhNamingCase SuffixNamingCase);

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
	 * @brief Returns blueprint type for given asset
	 * @param Asset FAssetData
	 * @return EGdhBlueprintType
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static EGdhBlueprintType GetBlueprintType(const FAssetData& Asset);

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

	/**
	 * @brief Renames given asset
	 * @param Asset FAssetData
	 * @param NewName FString
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gdh|Lib_Asset")
	static bool RenameAsset(const FAssetData& Asset, const FString& NewName);

	/**
	 * @brief Saves all unsaved assets in project
	 * @param bPromptToUser show confirmation window to user when saving
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset")
	static bool SaveAllAssets(const bool bPromptToUser);

private:
	static void GetSourceAndConfigFiles(TSet<FString>& Files);
};
