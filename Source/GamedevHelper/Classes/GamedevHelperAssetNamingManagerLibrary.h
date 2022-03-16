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
	/**
	 * @brief Renames asset by given settings
	 * @param Asset FAssetData
	 * @param Settings UGamedevHelperAssetNamingManagerSettings
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static void RenameAsset(const FAssetData& Asset, const UGamedevHelperAssetNamingManagerSettings& Settings);

	/**
	 * @brief Renames assets by given settings
	 * @param Assets TArray<FAssetData>
	 * @param Settings UGamedevHelperAssetNamingManagerSettings
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|AssetNamingManager")
	static void RenameAssets(const TArray<FAssetData>& Assets, const UGamedevHelperAssetNamingManagerSettings& Settings);

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
	 * - PascalCase01 => Pascal_Case_01
	 * - 01_my-string => 01_my_string
	 * - TextureUV01 => Texture_UV_01
	 * @param OriginalString FString
	 * @return FString
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
};
