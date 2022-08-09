// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperAssetLibrary.h"
// Engine Headers
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Misc/ScopedSlowTask.h"
#include "Misc/FeedbackContext.h"

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
