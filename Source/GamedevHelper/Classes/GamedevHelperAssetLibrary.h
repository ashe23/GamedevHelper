﻿// Copyright Ashot Barkhudaryan. All Rights Reserved.

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
	 * @brief Configures texture of specified type for vertex animation
	 * @see https://docs.unrealengine.com/4.27/en-US/AnimatingObjects/SkeletalMeshAnimation/Tools/VertexAnimationTool/
	 * @param Texture 
	 * @param TextureType EGamedevHelperVertexAnimTexture
	 * @return bool
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset|VertexAnimTool")
	static bool VertexAnimConfigureTexture(UTexture2D* Texture, const EGamedevHelperVertexAnimTexture TextureType);

	/**
	 * @brief Configures textures of specified type for vertex animation
	 * @see https://docs.unrealengine.com/4.27/en-US/AnimatingObjects/SkeletalMeshAnimation/Tools/VertexAnimationTool/
	 * @param Textures 
	 * @param TextureType EGamedevHelperVertexAnimTexture
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelper|Asset|VertexAnimTool")
	static void VertexAnimConfigureTextures(TArray<UTexture2D*> Textures, const EGamedevHelperVertexAnimTexture TextureType);

	// static void VertexAnimConfigureStaticMesh();

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
