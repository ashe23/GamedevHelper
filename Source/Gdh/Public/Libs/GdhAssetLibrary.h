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
	static void GetSelectedAssets(TArray<FAssetData>& Assets);

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

	template <class A>
	static void GetSelectedAssetsFiltered(TArray<A*>& Assets);
};
