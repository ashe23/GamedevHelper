// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperAssetLibrary.h"
#include "GamedevHelperNotificationLibrary.h"
// Engine Headers
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Misc/ScopedSlowTask.h"
#include "Engine/Texture2D.h"
#include "RenderingThread.h"
#include "Misc/FeedbackContext.h"
#include "EditorAssetLibrary.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

void UGamedevHelperAssetLibrary::SaveAll(const bool bUserPrompt)
{
	FEditorFileUtils::SaveDirtyPackages(
		bUserPrompt,
		true,
		true,
		false,
		false,
		false
	);
}

void UGamedevHelperAssetLibrary::FixupRedirectors(const FString& Path)
{
	const FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

	FScopedSlowTask FixRedirectorsTask{
		1.0f,
		FText::FromString(TEXT("Fixing up redirectors"))
	};
	FixRedirectorsTask.MakeDialog();

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace(Path);
	Filter.ClassNames.Emplace(UObjectRedirector::StaticClass()->GetFName());

	// Getting all redirectors in project
	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);

	if (AssetList.Num() > 0)
	{
		FScopedSlowTask FixRedirectorsLoadingTask(
			AssetList.Num(),
			FText::FromString(TEXT("Loading assets"))
		);
		FixRedirectorsLoadingTask.MakeDialog();

		TArray<UObjectRedirector*> Redirectors;
		Redirectors.Reserve(AssetList.Num());

		for (const auto& Asset : AssetList)
		{
			FixRedirectorsLoadingTask.EnterProgressFrame();

			UObject* AssetObj = Asset.GetAsset();
			if (!AssetObj) continue;

			const auto Redirector = CastChecked<UObjectRedirector>(AssetObj);
			if (!Redirector) continue;

			Redirectors.Add(Redirector);
		}

		Redirectors.Shrink();

		// Fix up all founded redirectors
		AssetToolsModule.Get().FixupReferencers(Redirectors);
	}

	FixRedirectorsTask.EnterProgressFrame(1.0f);
}

EGamedevHelperBlueprintType UGamedevHelperAssetLibrary::GetBlueprintType(const FAssetData& AssetData)
{
	if (!AssetData.IsValid())
	{
		return EGamedevHelperBlueprintType::None;
	}

	FString BlueprintTypeStr;
	AssetData.GetTagValue(TEXT("BlueprintType"), BlueprintTypeStr);

	FString ParentClassStr;
	AssetData.GetTagValue(TEXT("ParentClass"), ParentClassStr);

	if (BlueprintTypeStr.IsEmpty())
	{
		return EGamedevHelperBlueprintType::None;
	}

	const EBlueprintType BlueprintType = GetEnumValueFromString<EBlueprintType>(TEXT("EBlueprintType"), BlueprintTypeStr);

	if (BlueprintType == BPTYPE_Normal || BlueprintType == BPTYPE_Const)
	{
		return EGamedevHelperBlueprintType::Normal;
	}

	if (BlueprintType == BPTYPE_Interface)
	{
		return EGamedevHelperBlueprintType::Interface;
	}

	if (BlueprintType == BPTYPE_FunctionLibrary)
	{
		return EGamedevHelperBlueprintType::FunctionLibrary;
	}

	if (BlueprintType == BPTYPE_MacroLibrary)
	{
		return EGamedevHelperBlueprintType::MacroLibrary;
	}

	return EGamedevHelperBlueprintType::None;
}

bool UGamedevHelperAssetLibrary::IsBlueprint(const FAssetData& AssetData)
{
	return GetBlueprintType(AssetData) != EGamedevHelperBlueprintType::None;
}

UClass* UGamedevHelperAssetLibrary::GetBlueprintParentClass(const FAssetData& AssetData)
{
	if (!AssetData.IsValid()) return nullptr;

	const auto BlueprintAsset = Cast<UBlueprint>(AssetData.GetAsset());
	if (!BlueprintAsset) return nullptr;

	return BlueprintAsset->ParentClass;
}

bool UGamedevHelperAssetLibrary::VertexAnimConfigureTexture(UTexture2D* Texture, const EGamedevHelperVertexAnimTexture TextureType)
{
	if (!Texture) return false;

	Texture->SRGB = false;
	Texture->CompressionSettings = TextureType == EGamedevHelperVertexAnimTexture::UV ? TC_HDR : TC_VectorDisplacementmap;
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

void UGamedevHelperAssetLibrary::VertexAnimConfigureTextures(TArray<UTexture2D*> Textures, const EGamedevHelperVertexAnimTexture TextureType)
{
	if (!GWarn) return;
	if (Textures.Num() == 0)
	{
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Select textures"), EGamedevHelperModalStatus::Fail, 3.0f);
		return;
	}

	bool bResult = true;
	GWarn->BeginSlowTask(FText::FromString(TEXT("Configuring textures for vertex animation")), true);

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
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Textures configured successfully"), EGamedevHelperModalStatus::Success, 3.0f);
	}
	else
	{
		UGamedevHelperNotificationLibrary::ShowModalWithOutputLog(TEXT("Failed to configure some textures"));
	}
}

bool UGamedevHelperAssetLibrary::VertexAnimConfigureStaticMesh(UStaticMesh* StaticMesh)
{
	if (!StaticMesh) return false;
	if (!StaticMesh->IsValidLowLevel()) return false;

	DisableCollision(StaticMesh);

	StaticMesh->Modify();

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

void UGamedevHelperAssetLibrary::VertexAnimConfigureStaticMeshes(TArray<UStaticMesh*> StaticMeshes)
{
	if (!GWarn) return;
	if (StaticMeshes.Num() == 0)
	{
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Select static meshes"), EGamedevHelperModalStatus::Fail, 3.0f);
		return;
	}

	bool bResult = true;

	GWarn->BeginSlowTask(FText::FromString(TEXT("Configuring static meshes for vertex animation")), true);

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
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("StaticMeshes configured successfully"), EGamedevHelperModalStatus::Success, 3.0f);
	}
	else
	{
		UGamedevHelperNotificationLibrary::ShowModalWithOutputLog(TEXT("Failed to configure some StaticMeshes"));
	}
}

bool UGamedevHelperAssetLibrary::DisableCollision(UStaticMesh* StaticMesh)
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

void UGamedevHelperAssetLibrary::DisableCollisions(TArray<UStaticMesh*> StaticMeshes)
{
	if (!GWarn) return;
	if (StaticMeshes.Num() == 0)
	{
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Select static meshes"), EGamedevHelperModalStatus::Fail, 3.0f);
		return;
	}

	bool bResult = true;

	GWarn->BeginSlowTask(FText::FromString(TEXT("Disabling collisions")), true);

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
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Collisions disabled successfully"), EGamedevHelperModalStatus::Success, 3.0f);
	}
	else
	{
		UGamedevHelperNotificationLibrary::ShowModalWithOutputLog(TEXT("Failed to disable collisions"));
	}
}

void UGamedevHelperAssetLibrary::GetSelectedAssets(TArray<FAssetData>& Assets)
{
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	ContentBrowserModule.Get().GetSelectedAssets(Assets);
}
