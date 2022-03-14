// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperCoreAssetLibrary.h"
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

EGDHBlueprintType UGamedevHelperAssetLibrary::GetBlueprintType(const FAssetData& AssetData)
{
	if (!AssetData.IsValid())
	{
		return EGDHBlueprintType::None;
	}

	FString BlueprintTypeStr;
	AssetData.GetTagValue(TEXT("BlueprintType"), BlueprintTypeStr);

	if (BlueprintTypeStr.IsEmpty())
	{
		return EGDHBlueprintType::None;
	}

	const EBlueprintType BlueprintType = GetEnumValueFromString<EBlueprintType>(TEXT("EBlueprintType"), BlueprintTypeStr);

	switch (BlueprintType)
	{
	case BPTYPE_Normal:
	case BPTYPE_Const:
		return EGDHBlueprintType::Normal;
	case BPTYPE_FunctionLibrary:
		return EGDHBlueprintType::FunctionLibrary;
	case BPTYPE_Interface:
		return EGDHBlueprintType::Interface;
	case BPTYPE_MacroLibrary:
		return EGDHBlueprintType::MacroLibrary;
	default:
		return EGDHBlueprintType::None;
	}
}

bool UGamedevHelperAssetLibrary::IsBlueprint(const FAssetData& AssetData)
{
	return GetBlueprintType(AssetData) != EGDHBlueprintType::None;
}

void UGamedevHelperAssetLibrary::VertexAnimToolConfigureTexture(const TArray<FAssetData>& Assets, const EGDHVertexAnimToolTextureType TextureType)
{
	if (!GWarn) return;

	GWarn->BeginSlowTask(FText::FromString(TEXT("[GDH_VertexAnimTool] Configuring textures")), true);

	for (const auto& Asset : Assets)
	{
		const auto Texture = Cast<UTexture2D>(Asset.GetAsset());
		if (!Texture) continue;

		Texture->SRGB = false;
		Texture->CompressionSettings = TextureType == EGDHVertexAnimToolTextureType::UV ? TC_HDR : TC_VectorDisplacementmap;
		Texture->Filter = TF_Nearest;
		Texture->NeverStream = true;
		Texture->MipGenSettings = TMGS_NoMipmaps;
		Texture->LODGroup = TEXTUREGROUP_UI;
		Texture->PostEditChange();

		if (Texture->MarkPackageDirty())
		{
			UEditorAssetLibrary::SaveAsset(Asset.ObjectPath.ToString());
		}
	}

	GWarn->EndSlowTask();
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
