// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetNamingManagerLibrary.generated.h"

class UGamedevHelperAssetNamingConventionSettings;

UCLASS()
class UGamedevHelperAssetNamingManagerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Renames assets by given settings
	 * @param Assets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static void RenameAssets(const TArray<FAssetData>& Assets);

	/**
	 * @brief Returns rename previews for given assets. Does not rename assets.
	 * @param Assets TArray<FAssetData>
	 * @param NamingConvention UGamedevHelperAssetNamingConventionSettings
	 * @param Previews TArray<FGamedevHelperRenamePreview>
	 */
	static void GetRenamePreviews(const TArray<FAssetData>& Assets,
	                              const UGamedevHelperAssetNamingConventionSettings* NamingConvention,
	                              TArray<FGamedevHelperRenamePreview>& Previews);

	/**
	 * @brief Returns normalized string by removing all extra underscores and hyphens from string start and end, then replaces by underscore in the middle of string 
	 * - _aa_ => aa
	 * - aaa => aaa
	 * - aaa___bbb___ccc => aaa_bbb_ccc
	 * - -aa-bb-cc--01- => aa_bb_cc_01
	 * 
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString Normalize(const FString& OriginalString);

	/**
	 * @brief Returns new string with contextual parts splitted by underscore. String can be in any naming case, even mixed.
	 * - PascalCase01 => pascal_case_01
	 * - 01_my-string => 01_my_string
	 * - TextureUV01 => texture_uv_01
	 * @param OriginalString FString
	 * @return FString Result is always in snake_case
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString Tokenize(const FString& OriginalString);

	/**
	 * @brief Converts string to given naming case
	 * @param OriginalString FString
	 * @param NamingCase EGamedevHelperNamingCase
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertNamingCase(const FString& OriginalString, const EGamedevHelperNamingCase NamingCase);

	/**
	 * @brief Converts string to PascalCase
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertToPascalCase(const FString& OriginalString);

	/**
	 * @brief Converts string to Pascal_Snake_Case
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertToPascalSnakeCase(const FString& OriginalString);

	/**
	 * @brief Converts string to snake_case
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertToSnakeCase(const FString& OriginalString);

	/**
	 * @brief Converts string to kebab-case
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertToKebabCase(const FString& OriginalString);

	/**
	 * @brief Converts string to camelCase
	 * @param OriginalString FString
	 * @return FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static FString ConvertToCamelCase(const FString& OriginalString);

private:
	/**
	 * @brief Removes old prefixes and suffixes from old asset name. Asset name must be normalized.
	 * @param OldAssetName FString
	 * @param NamingConvention UGamedevHelperAssetNamingConvention
	 * @return FString
	 */
	static FString RemoveOldPrefixAndSuffix(const FString& OldAssetName,
	                                        const UGamedevHelperAssetNamingConventionSettings* NamingConvention);
};
