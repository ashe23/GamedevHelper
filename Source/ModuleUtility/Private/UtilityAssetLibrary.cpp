// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UtilityAssetLibrary.h"
// #include "ModuleUtility.h"
// Engine Headers
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Misc/ScopedSlowTask.h"
// #include "ObjectTools.h"

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return static_cast<EnumType>(Enum->GetIndexByName(FName(*String)));
}

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
