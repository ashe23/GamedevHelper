// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperAssetLibrary.generated.h"

UCLASS()
class UGamedevHelperAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief Saves all unsaved assets
	 * @param bUserPrompt bool - show user confirmation dialog or not
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static void SaveAll(const bool bUserPrompt = true);

	/**
	 * @brief Fixing up all redirectors in given path
	 * @param Path FString
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static void FixupRedirectors(const FString& Path);

	/**
	 * @brief Return blueprint type
	 * @param AssetData FAssetData
	 * @return EGDHBlueprintType
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static EGamedevHelperBlueprintType GetBlueprintType(const FAssetData& AssetData);

	/**
	 * @brief Checks if given asset is blueprint or not
	 * @param AssetData FAssetData
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static bool IsBlueprint(const FAssetData& AssetData);

	/**
	 * @brief Returns blueprint asset parent class
	 * @param AssetData FAssetData
	 * @return UClass*
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static UClass* GetBlueprintParentClass(const FAssetData& AssetData);
};
