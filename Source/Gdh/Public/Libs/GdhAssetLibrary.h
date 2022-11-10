// Copyright Ashot Barkhudaryan All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhTypes.h"
#include "GdhAssetLibrary.generated.h"

class UGdhAssetNamingConventionSettings;
struct FGdhRenamePreview;

UCLASS()
class UGdhAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset", meta=(ToolTip="Returns all selected assets in content browser"))
	static void GetAssetsSelected(TArray<FAssetData>& Assets);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset", meta=(ToolTip="Returns all assets under /Game folder"))
	static void GetAssetsAll(TArray<FAssetData>& Assets);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset", meta=(ToolTip="Returns all assets in given path"))
	static void GetAssetsInPath(const FString& Path, TArray<FAssetData>& Assets, const bool bRecursiveSearch = false);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset", meta=(ToolTip="Returns all assets by given filter"))
	static void GetAssetsByFilter(const FARFilter& Filter, TArray<FAssetData>& Assets);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset|VertexAnimTool")
	static bool VertexAnimConfigureTexture(UTexture2D* Texture, const EGdhVertexAnimTextureType TextureType);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset|VertexAnimTool")
	static void VertexAnimConfigureTextures(TArray<UTexture2D*> Textures, const EGdhVertexAnimTextureType TextureType);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset|VertexAnimTool")
	static bool VertexAnimConfigureStaticMesh(UStaticMesh* StaticMesh);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset|VertexAnimTool")
	static void VertexAnimConfigureStaticMeshes(TArray<UStaticMesh*> StaticMeshes);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset")
	static bool DisableCollision(UStaticMesh* StaticMesh);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset")
	static void DisableCollisions(TArray<UStaticMesh*> StaticMeshes);
	
	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset")
	static UClass* GetBlueprintParentClass(const FAssetData& AssetData);

	UFUNCTION(BlueprintCallable, Category="Gdh|Lib_Asset")
	static FGdhAssetNameParts GetAssetNameParts(const FAssetData& AssetData);

	template <class A>
	static void GetSelectedAssetsFiltered(TArray<A*>& Assets);
};
