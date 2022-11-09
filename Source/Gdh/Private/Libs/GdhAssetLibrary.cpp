// Copyright Ashot Barkhudaryan All Rights Reserved.

#include "Libs/GdhAssetLibrary.h"
#include "Libs/GdhNotificationLibrary.h"
#include "Settings/GdhAssetNamingConventionSettings.h"
#include "UI/GdhAssetNamingManagerRenamePreview.h"
// Engine Headers
#include "ContentBrowserModule.h"
#include "EditorAssetLibrary.h"
#include "IContentBrowserSingleton.h"
#include "Libs/GdhStringLibrary.h"
#include "Misc/FeedbackContext.h"

void UGdhAssetLibrary::GetSelectedAssets(TArray<FAssetData>& Assets)
{
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	ContentBrowserModule.Get().GetSelectedAssets(Assets);
}

bool UGdhAssetLibrary::VertexAnimConfigureTexture(UTexture2D* Texture, const EGdhVertexAnimTextureType TextureType)
{
	if (!Texture) return false;

	Texture->SRGB = false;
	Texture->CompressionSettings = TextureType == EGdhVertexAnimTextureType::UV ? TC_HDR : TC_VectorDisplacementmap;
	Texture->Filter = TF_Nearest;
	Texture->NeverStream = true;
	Texture->MipGenSettings = TMGS_NoMipmaps;
	Texture->LODGroup = TEXTUREGROUP_UI;
	Texture->PostEditChange();

	if (Texture->MarkPackageDirty())
	{
		return UEditorAssetLibrary::SaveLoadedAsset(Texture, true);
	}

	return false;
}

void UGdhAssetLibrary::VertexAnimConfigureTextures(TArray<UTexture2D*> Textures, const EGdhVertexAnimTextureType TextureType)
{
	if (!GWarn || Textures.Num() == 0) return;

	const FString Title = FString::Printf(TEXT("Configuring %s for vertex animation"), Textures.Num() > 1 ? TEXT("textures") : TEXT("texture"));
	const FString MsgSuccess = FString::Printf(TEXT("Done. Configured %d %s for vertex animation"), Textures.Num(), Textures.Num() > 1 ? TEXT("textures") : TEXT("texture"));
	const FString MsgError = FString::Printf(TEXT("Error. Failed to configure some %s"), Textures.Num() > 1 ? TEXT("textures") : TEXT("texture"));

	bool bResult = true;
	GWarn->BeginSlowTask(FText::FromString(Title), true);

	for (const auto& Texture : Textures)
	{
		if (!VertexAnimConfigureTexture(Texture, TextureType))
		{
			bResult = false;
		}
	}

	GWarn->EndSlowTask();

	if (bResult)
	{
		UGdhNotificationLibrary::ShowModal(TEXT("Asset Library"), MsgSuccess, EGdhModalStatus::OK, 3.0f);
	}
	else
	{
		UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Asset Library"), MsgError);
	}
}

bool UGdhAssetLibrary::VertexAnimConfigureStaticMesh(UStaticMesh* StaticMesh)
{
	if (!StaticMesh) return false;
	if (!StaticMesh->IsValidLowLevel()) return false;

	DisableCollision(StaticMesh);

	StaticMesh->Modify();

	// disabling nanite on mesh, because vertex animation currently not working with it
	StaticMesh->NaniteSettings.bEnabled = false;

	for (int32 LODIndex = 0; LODIndex < StaticMesh->GetNumLODs(); ++LODIndex)
	{
		FStaticMeshSourceModel& SourceModel = StaticMesh->GetSourceModel(LODIndex);
		SourceModel.BuildSettings.bRemoveDegenerates = false;
		SourceModel.BuildSettings.bUseFullPrecisionUVs = true;
		SourceModel.BuildSettings.bGenerateLightmapUVs = false;
		SourceModel.BuildSettings.DistanceFieldResolutionScale = 0.0f;
	}
	StaticMesh->PostEditChange();

	if (StaticMesh->MarkPackageDirty())
	{
		return UEditorAssetLibrary::SaveLoadedAsset(StaticMesh, true);
	}

	return false;
}

void UGdhAssetLibrary::VertexAnimConfigureStaticMeshes(TArray<UStaticMesh*> StaticMeshes)
{
	if (!GWarn || StaticMeshes.Num() == 0) return;

	const FString Title = FString::Printf(TEXT("Configuring static %s for vertex animation"), StaticMeshes.Num() > 1 ? TEXT("meshes") : TEXT("mesh"));
	const FString MsgSuccess = FString::Printf(TEXT("Done. Configured %d Static %s for vertex animation"), StaticMeshes.Num(), StaticMeshes.Num() > 1 ? TEXT("meshes") : TEXT("mesh"));
	const FString MsgError = FString::Printf(TEXT("Error: Failed to configure some static %s for vertex animation"), StaticMeshes.Num() > 1 ? TEXT("meshes") : TEXT("mesh"));

	bool bResult = true;
	GWarn->BeginSlowTask(FText::FromString(Title), true);

	for (const auto& StaticMesh : StaticMeshes)
	{
		if (!VertexAnimConfigureStaticMesh(StaticMesh))
		{
			bResult = false;
		}
	}

	GWarn->EndSlowTask();

	if (bResult)
	{
		UGdhNotificationLibrary::ShowModal(TEXT("Asset Library"), MsgSuccess, EGdhModalStatus::OK, 3.0f);
	}
	else
	{
		UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Asset Library"), MsgError);
	}
}

bool UGdhAssetLibrary::DisableCollision(UStaticMesh* StaticMesh)
{
	if (!StaticMesh) return false;
	if (!StaticMesh->IsValidLowLevel()) return false;

	UBodySetup* BodySetup = StaticMesh->GetBodySetup();
	if (!BodySetup) return false;

	StaticMesh->Modify();

	FBodyInstance BodyInstance = StaticMesh->GetBodySetup()->DefaultInstance;
	BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyInstance.SetCollisionProfileName(FName{"NoCollision"});

	BodySetup->DefaultInstance = BodyInstance;
	StaticMesh->SetBodySetup(BodySetup);
	StaticMesh->PostEditChange();

	if (StaticMesh->MarkPackageDirty())
	{
		return UEditorAssetLibrary::SaveLoadedAsset(StaticMesh, true);
	}

	return false;
}

void UGdhAssetLibrary::DisableCollisions(TArray<UStaticMesh*> StaticMeshes)
{
	if (!GWarn || StaticMeshes.Num() == 0) return;

	const FString Title = FString::Printf(TEXT("Disabling %s"), StaticMeshes.Num() > 1 ? TEXT("collisions") : TEXT("collision"));
	const FString MsgSuccess = FString::Printf(TEXT("Done. Disabled collision on %d static %s"), StaticMeshes.Num(), StaticMeshes.Num() > 1 ? TEXT("meshes") : TEXT("mesh"));
	const FString MsgError = FString::Printf(TEXT("Error. Failed to disable collision on some static %s"), StaticMeshes.Num() > 1 ? TEXT("meshes") : TEXT("mesh"));

	bool bResult = true;
	GWarn->BeginSlowTask(FText::FromString(Title), true);

	for (const auto& StaticMesh : StaticMeshes)
	{
		if (!DisableCollision(StaticMesh))
		{
			bResult = false;
		}
	}

	GWarn->EndSlowTask();

	if (bResult)
	{
		UGdhNotificationLibrary::ShowModal(TEXT("Asset Library"), MsgSuccess, EGdhModalStatus::OK, 3.0f);
	}
	else
	{
		UGdhNotificationLibrary::ShowModalOutputLog(TEXT("Asset Library"), MsgError);
	}
}

UClass* UGdhAssetLibrary::GetBlueprintParentClass(const FAssetData& AssetData)
{
	if (!AssetData.IsValid()) return nullptr;

	const auto BlueprintAsset = Cast<UBlueprint>(AssetData.GetAsset());
	if (!BlueprintAsset) return nullptr;

	return BlueprintAsset->ParentClass;
}

template <class A>
void UGdhAssetLibrary::GetSelectedAssetsFiltered(TArray<A*>& Assets)
{
	TArray<FAssetData> SelectedAssets;
	GetSelectedAssets(SelectedAssets);

	Assets.Reset();
	Assets.Reserve(SelectedAssets.Num());

	for (const auto& SelectedAsset : SelectedAssets)
	{
		A* CastedAsset = Cast<A>(SelectedAsset.GetAsset());
		if (!CastedAsset) continue;

		Assets.Add(CastedAsset);
	}
}
