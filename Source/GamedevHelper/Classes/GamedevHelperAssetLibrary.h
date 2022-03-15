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
	 * @brief Configures selected texture assets for vertex animation
	 * @param Assets TArray<FAssetData>
	 * @param TextureType EGDHVertexAnimToolTexture
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset|VertexAnimTool")
	static void VertexAnimToolConfigureTexture(const TArray<FAssetData>& Assets, const EGamedevHelperVertexAnimTexture TextureType);

	/**
	 * @brief Configures selected static meshes for vertex animation, including LODs
	 * @param Assets TArray<FAssetData>
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset|VertexAnimTool")
	static void VertexAnimToolConfigureStaticMesh(const TArray<FAssetData>& Assets);

	/**
	 * @brief Disables collision on given static mesh assets
	 * @param Assets TArray<FAssetData>
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset")
	static void DisableCollision(const TArray<FAssetData>& Assets);
};