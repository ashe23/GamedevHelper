// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperAssetLibrary.h"
// Engine Headers
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Misc/ScopedSlowTask.h"
#include "Engine/Texture2D.h"
#include "RenderingThread.h"
#include "Misc/FeedbackContext.h"
#include "EditorAssetLibrary.h"

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

	// todo:ashe23 show notification based on result
}

void UGamedevHelperAssetLibrary::VertexAnimToolConfigureStaticMesh(const TArray<FAssetData>& Assets)
{
	if (!GWarn) return;

	DisableCollision(Assets);

	GWarn->BeginSlowTask(FText::FromString(TEXT("[GDH_VertexAnimTool] Configuring static meshes")), true);
	FlushRenderingCommands();

	for (const auto& Asset : Assets)
	{
		const auto StaticMesh = Cast<UStaticMesh>(Asset.GetAsset());
		if (!StaticMesh) continue;

		StaticMesh->Modify();

		for (int32 LODIndex = 0; LODIndex < StaticMesh->GetNumLODs(); ++LODIndex)
		{
			if (!StaticMesh->IsSourceModelValid(LODIndex)) continue;

			FStaticMeshSourceModel& SourceModel = StaticMesh->GetSourceModel(LODIndex);
			SourceModel.BuildSettings.bRemoveDegenerates = false;
			SourceModel.BuildSettings.bUseFullPrecisionUVs = true;
			SourceModel.BuildSettings.bGenerateLightmapUVs = false;
			SourceModel.BuildSettings.DistanceFieldResolutionScale = 0.0f;
		}

		StaticMesh->PostEditChange();

		if (StaticMesh->MarkPackageDirty())
		{
			UEditorAssetLibrary::SaveAsset(Asset.ObjectPath.ToString());
		}
	}

	GWarn->EndSlowTask();
}

void UGamedevHelperAssetLibrary::DisableCollision(const TArray<FAssetData>& Assets)
{
	if (!GWarn) return;

	GWarn->BeginSlowTask(FText::FromString(TEXT("Disabling collision")), true);

	for (const auto& Asset : Assets)
	{
		const auto StaticMesh = Cast<UStaticMesh>(Asset.GetAsset());
		if (!StaticMesh) continue;

		UBodySetup* BodySetup = StaticMesh->GetBodySetup();
		if (!BodySetup) continue;

		StaticMesh->Modify();

		FBodyInstance BodyInstance = StaticMesh->GetBodySetup()->DefaultInstance;
		BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BodyInstance.SetCollisionProfileName(FName{"NoCollision"});

		BodySetup->DefaultInstance = BodyInstance;
		StaticMesh->SetBodySetup(BodySetup);

		StaticMesh->PostEditChange();

		if (StaticMesh->MarkPackageDirty())
		{
			UEditorAssetLibrary::SaveAsset(Asset.ObjectPath.ToString());
		}
	}

	GWarn->EndSlowTask();
}
