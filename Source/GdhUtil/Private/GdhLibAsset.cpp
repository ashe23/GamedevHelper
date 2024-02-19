// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibAsset.h"
#include "GdhLibEditor.h"
#include "GdhConstants.h"
#include "GdhLibPath.h"
#include "GdhLibString.h"
#include "GdhStructs.h"
#include "GdhUtilModule.h"
// Engine Headers
#include "AssetToolsModule.h"
#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"
#include "Internationalization/Regex.h"
#include "Misc/FileHelper.h"
#include "Misc/ScopedSlowTask.h"

void UGdhLibAsset::GetAssetsAll(TArray<FAssetData>& Assets)
{
	if (UGdhLibEditor::GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	Assets.Reset();

	UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetsByPath(GdhConstants::PathRoot, Assets, true);
}

void UGdhLibAsset::GetAssetByPath(const FString& Path, const bool bRecursive, TArray<FAssetData>& Assets)
{
	if (UGdhLibEditor::GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	Assets.Reset();

	UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetsByPath(FName{*Path}, Assets, bRecursive);
}

void UGdhLibAsset::GetAssetsPrimary(TArray<FAssetData>& Assets, const bool bShowSlowTask)
{
	if (UGdhLibEditor::GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	TArray<FAssetData> AssetsAll;
	GetAssetsAll(AssetsAll);

	TSet<FName> ClassNamesPrimary;
	GetClassNamesPrimary(ClassNamesPrimary);

	Assets.Reset(AssetsAll.Num());

	FScopedSlowTask SlowTask{
		static_cast<float>(AssetsAll.Num()),
		FText::FromString(TEXT("Searching primary assets...")),
		bShowSlowTask && GIsEditor && !IsRunningCommandlet()
	};
	SlowTask.MakeDialog(false, false);

	for (const auto& Asset : AssetsAll)
	{
		SlowTask.EnterProgressFrame(1.0f, FText::FromString(Asset.GetFullName()));

		if (ClassNamesPrimary.Contains(Asset.AssetClass) || ClassNamesPrimary.Contains(GetAssetExactClassName(Asset)))
		{
			Assets.Emplace(Asset);
		}
	}

	Assets.Shrink();
}

void UGdhLibAsset::GetAssetsIndirect(TArray<FAssetData>& Assets, const bool bShowSlowTask)
{
	if (UGdhLibEditor::GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	Assets.Reset();

	TSet<FString> ScanFiles;
	GetSourceAndConfigFiles(ScanFiles);

	FScopedSlowTask SlowTask{
		static_cast<float>(ScanFiles.Num()),
		FText::FromString(TEXT("Searching Indirectly used assets...")),
		bShowSlowTask && GIsEditor && !IsRunningCommandlet()
	};
	SlowTask.MakeDialog(false, false);

	for (const auto& File : ScanFiles)
	{
		SlowTask.EnterProgressFrame(1.0f, FText::FromString(File));

		FString FileContent;
		FFileHelper::LoadFileToString(FileContent, *File);

		if (FileContent.IsEmpty()) continue;

		static FRegexPattern Pattern(TEXT(R"(\/Game([A-Za-z0-9_.\/]+)\b)"));
		FRegexMatcher Matcher(Pattern, FileContent);
		while (Matcher.FindNext())
		{
			FString FoundedAssetObjectPath = Matcher.GetCaptureGroup(0);

			const FString ObjectPath = UGdhLibPath::PathConvertToObjectPath(FoundedAssetObjectPath);
			if (ObjectPath.IsEmpty()) continue;

			const FAssetData AssetData = UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetByObjectPath(FName{*ObjectPath});
			if (!AssetData.IsValid()) continue;

			Assets.AddUnique(AssetData);
		}
	}
}

void UGdhLibAsset::GetAssetsUnicode(TArray<FAssetData>& Assets, const bool bShowSlowTask)
{
	TArray<FAssetData> AssetsAll;
	GetAssetsAll(AssetsAll);

	Assets.Reset(AssetsAll.Num());

	FScopedSlowTask SlowTask{
		static_cast<float>(AssetsAll.Num()),
		FText::FromString(TEXT("Searching assets with unicode characters in name...")),
		bShowSlowTask && GIsEditor && !IsRunningCommandlet()
	};
	SlowTask.MakeDialog(false, false);

	for (const auto& Asset : AssetsAll)
	{
		SlowTask.EnterProgressFrame(1.0f, FText::FromName(Asset.AssetName));

		if (UGdhLibString::HasUnicode(Asset.AssetName.ToString()))
		{
			Assets.Add(Asset);
		}
	}

	Assets.Shrink();
}

void UGdhLibAsset::GetClassNamesPrimary(TSet<FName>& ClassNames)
{
	// getting list of primary asset classes that are defined in AssetManager
	const auto& AssetManager = UAssetManager::Get();
	if (!AssetManager.IsValid()) return;

	TSet<FName> ClassNamesPrimaryBase;
	TArray<FPrimaryAssetTypeInfo> AssetTypeInfos;
	AssetManager.Get().GetPrimaryAssetTypeInfoList(AssetTypeInfos);
	ClassNamesPrimaryBase.Reserve(AssetTypeInfos.Num());

	for (const auto& AssetTypeInfo : AssetTypeInfos)
	{
		if (!AssetTypeInfo.AssetBaseClassLoaded) continue;

		ClassNamesPrimaryBase.Emplace(AssetTypeInfo.AssetBaseClassLoaded->GetFName());
	}

	// getting list of primary assets classes that are derived from main primary assets
	ClassNames.Empty();
	UGdhLibEditor::GetModuleAssetRegistry().Get().GetDerivedClassNames(ClassNamesPrimaryBase.Array(), TSet<FName>{}, ClassNames);
}


FName UGdhLibAsset::GetAssetExactClassName(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return NAME_None;

	if (AssetIsBlueprint(Asset))
	{
		const FString GeneratedClassName = Asset.TagsAndValues.FindTag(TEXT("GeneratedClass")).GetValue();
		const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassName);
		return FName{*FPackageName::ObjectPathToObjectName(ClassObjectPath)};
	}

	return Asset.AssetClass;
}

FString UGdhLibAsset::GetAssetTagValue(const FAssetData& Asset, const FName& Tag)
{
	if (!Asset.IsValid()) return {};
	if (!Asset.TagsAndValues.Contains(Tag)) return {};

	const FString Value = Asset.TagsAndValues.FindTag(Tag).GetValue();
	return FPackageName::ExportTextPathToObjectPath(Value);
}

FGdhAssetNameAffix UGdhLibAsset::GetAssetNameAffix(const FAssetData& Asset, const UDataTable* Mappings, const TMap<EGdhBlueprintType, FGdhAssetNameAffix>& BlueprintTypes)
{
	if (!Asset.IsValid()) return {};
	if (!Mappings) return {};

	TArray<FGdhAssetNameAffixRow*> Rows;
	Mappings->GetAllRows<FGdhAssetNameAffixRow>(TEXT(""), Rows);

	TMap<FString, FGdhAssetNameAffix> AffixMap;
	AffixMap.Reserve(Rows.Num());
	
	for (const auto& Row : Rows)
	{
		if (!Row) continue;
		if (!Row->AssetClass.LoadSynchronous()) continue;

		AffixMap.Add(Row->AssetClass.ToString(), FGdhAssetNameAffix{Row->Prefix, Row->Suffix});
	}

	const bool bIsBlueprint = AssetIsBlueprint(Asset);
	const EGdhBlueprintType BlueprintType = GetBlueprintType(Asset);
	const FGdhAssetNameAffix BlueprintAffix = BlueprintTypes.Contains(BlueprintType) ? *BlueprintTypes.Find(BlueprintType) : FGdhAssetNameAffix{};
	const FString AssetExactClassName = bIsBlueprint ? GetAssetTagValue(Asset, TEXT("GeneratedClass")) : FSoftClassPath(Asset.GetClass()).GetAssetPathString();
	const FString AssetParentClassName = bIsBlueprint ? GetAssetTagValue(Asset, TEXT("ParentClass")) : TEXT("");

	if (bIsBlueprint)
	{
		const FGdhAssetNameAffix ExactAffixes = AffixMap.Contains(AssetExactClassName) ? *AffixMap.Find(AssetExactClassName) : FGdhAssetNameAffix{};
		const FGdhAssetNameAffix ParentAffixes = AffixMap.Contains(AssetParentClassName) ? *AffixMap.Find(AssetParentClassName) : BlueprintAffix;

		return FGdhAssetNameAffix{ExactAffixes.Prefix.IsEmpty() ? ParentAffixes.Prefix : ExactAffixes.Prefix, ExactAffixes.Suffix.IsEmpty() ? ParentAffixes.Suffix : ExactAffixes.Suffix};
	}

	if (AffixMap.Contains(AssetExactClassName))
	{
		return *AffixMap.Find(AssetExactClassName);
	}

	return {};
}

FString UGdhLibAsset::GetAssetNameByConvention(const FString& Name, const FGdhAssetNameAffix& Affix, const EGdhNamingCase AssetNamingCase, const EGdhNamingCase PrefixNamingCase, const EGdhNamingCase SuffixNamingCase)
{
	if (Name.IsEmpty()) return Name;

	FString TokenizedName = UGdhLibString::Tokenize(Name);
	const FString DelimiterChar = TEXT("_");

	if (!Affix.Prefix.IsEmpty())
	{
		TokenizedName.RemoveFromStart(Affix.Prefix + DelimiterChar);
	}

	if (!Affix.Suffix.IsEmpty())
	{
		TokenizedName.RemoveFromEnd(DelimiterChar + Affix.Suffix);
	}

	const FString AssetNameOnly = UGdhLibString::ConvertNamingCase(TokenizedName, AssetNamingCase);
	const FString Prefix = Affix.Prefix.IsEmpty() ? TEXT("") : UGdhLibString::ConvertNamingCase(Affix.Prefix, PrefixNamingCase);
	const FString Suffix = Affix.Suffix.IsEmpty() ? TEXT("") : UGdhLibString::ConvertNamingCase(Affix.Suffix, SuffixNamingCase);

	const FString FinalName = FString::Printf(
		TEXT("%s%s%s"),
		Prefix.IsEmpty() ? TEXT("") : *(Prefix + DelimiterChar),
		*AssetNameOnly,
		Suffix.IsEmpty() ? TEXT("") : *(DelimiterChar + Suffix)
	);

	return FinalName;
}

int64 UGdhLibAsset::GetAssetSize(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return 0;

	const FAssetPackageData* AssetPackageData = UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetPackageData(Asset.PackageName);
	if (!AssetPackageData) return 0;

	return AssetPackageData->DiskSize;
}

int64 UGdhLibAsset::GetAssetsTotalSize(const TArray<FAssetData>& Assets)
{
	int64 Size = 0;

	for (const auto& Asset : Assets)
	{
		if (!Asset.IsValid()) continue;

		const auto AssetPackageData = UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssetPackageData(Asset.PackageName);
		if (!AssetPackageData) continue;

		Size += AssetPackageData->DiskSize;
	}

	return Size;
}

EGdhBlueprintType UGdhLibAsset::GetBlueprintType(const FAssetData& Asset)
{
	if (!Asset.IsValid())
	{
		return EGdhBlueprintType::None;
	}

	FString BlueprintTypeStr;
	Asset.GetTagValue(TEXT("BlueprintType"), BlueprintTypeStr);

	FString ParentClassStr;
	Asset.GetTagValue(TEXT("ParentClass"), ParentClassStr);

	if (BlueprintTypeStr.IsEmpty())
	{
		return EGdhBlueprintType::None;
	}

	const EBlueprintType BlueprintType = static_cast<EBlueprintType>(StaticEnum<EBlueprintType>()->GetValueByName(FName(*BlueprintTypeStr)));

	if (BlueprintType == BPTYPE_Normal || BlueprintType == BPTYPE_Const)
	{
		return EGdhBlueprintType::Normal;
	}

	if (BlueprintType == BPTYPE_Interface)
	{
		return EGdhBlueprintType::Interface;
	}

	if (BlueprintType == BPTYPE_FunctionLibrary)
	{
		return EGdhBlueprintType::FunctionLibrary;
	}

	if (BlueprintType == BPTYPE_MacroLibrary)
	{
		return EGdhBlueprintType::MacroLibrary;
	}

	return EGdhBlueprintType::None;
}

bool UGdhLibAsset::AssetIsBlueprint(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return false;

	const UClass* AssetClass = Asset.GetClass();
	if (!AssetClass) return false;

	return AssetClass->IsChildOf(UBlueprint::StaticClass());
}

bool UGdhLibAsset::AssetIsExtReferenced(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return false;

	TArray<FName> Refs;
	UGdhLibEditor::GetModuleAssetRegistry().Get().GetReferencers(Asset.PackageName, Refs);

	return Refs.ContainsByPredicate([](const FName& Ref)
	{
		return !Ref.ToString().StartsWith(GdhConstants::PathRoot.ToString());
	});
}

bool UGdhLibAsset::AssetIsCircular(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return false;

	TArray<FName> Refs;
	TArray<FName> Deps;

	UGdhLibEditor::GetModuleAssetRegistry().Get().GetReferencers(Asset.PackageName, Refs);
	UGdhLibEditor::GetModuleAssetRegistry().Get().GetDependencies(Asset.PackageName, Deps);

	for (const auto& Ref : Refs)
	{
		if (Deps.Contains(Ref))
		{
			return true;
		}
	}

	return false;
}

bool UGdhLibAsset::AssetHasRefs(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return false;

	TArray<FName> Refs;
	UGdhLibEditor::GetModuleAssetRegistry().Get().GetReferencers(Asset.PackageName, Refs);

	return Refs.Num() > 0;
}

void UGdhLibAsset::GetProjectRedirectors(TArray<FAssetData>& Redirectors)
{
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace(GdhConstants::PathRoot);
	Filter.ClassNames.Emplace(UObjectRedirector::StaticClass()->GetFName());

	Redirectors.Reset();
	UGdhLibEditor::GetModuleAssetRegistry().Get().GetAssets(Filter, Redirectors);
}

bool UGdhLibAsset::ProjectHasRedirectors()
{
	TArray<FAssetData> Redirectors;
	GetProjectRedirectors(Redirectors);

	return Redirectors.Num() > 0;
}

void UGdhLibAsset::FixProjectRedirectors(const TArray<FAssetData>& Redirectors, const bool bShowSlowTask)
{
	if (Redirectors.Num() == 0) return;

	FScopedSlowTask SlowTask{
		static_cast<float>(Redirectors.Num()),
		FText::FromString(TEXT("Fixing redirectors...")),
		bShowSlowTask && GIsEditor && !IsRunningCommandlet()
	};
	SlowTask.MakeDialog(false, false);

	TArray<UObjectRedirector*> RedirectorObjects;
	RedirectorObjects.Reserve(Redirectors.Num());

	for (const auto& Redirector : Redirectors)
	{
		SlowTask.EnterProgressFrame(1.0f, FText::FromString(Redirector.GetFullName()));

		UObjectRedirector* RedirectorObject = CastChecked<UObjectRedirector>(Redirector.GetAsset());
		if (!RedirectorObject) continue;

		RedirectorObjects.Emplace(RedirectorObject);
	}

	UGdhLibEditor::GetModuleAssetTools().Get().FixupReferencers(RedirectorObjects, false);
}

bool UGdhLibAsset::RenameAsset(const FAssetData& Asset, const FString& NewName)
{
	if (!Asset.IsValid())
	{
		UE_LOG(LogGdhUtil, Warning, TEXT("Failed To Rename. Invalid asset data."))
		return false;
	}

	if (NewName.IsEmpty())
	{
		const FString ErrMsg = FString::Printf(TEXT("Failed To Rename %s asset. Name cant be empty."), *Asset.AssetName.ToString());
		UE_LOG(LogGdhUtil, Warning, TEXT("%s"), *ErrMsg)
		return false;
	}

	if (!UGdhLibString::HasOnly(NewName, GdhConstants::ValidAssetNameChars))
	{
		const FString ErrMsg = FString::Printf(TEXT("Failed To Rename %s asset. Name contains invalid characters."), *Asset.AssetName.ToString());
		UE_LOG(LogGdhUtil, Warning, TEXT("%s"), *ErrMsg)
		return false;
	}

	const FString NewObjectPath = FString::Printf(TEXT("%s/%s.%s"), *Asset.PackagePath.ToString(), *NewName, *NewName);

	FAssetRenameData RenameData;
	RenameData.Asset = Asset.GetAsset();
	RenameData.OldObjectPath = Asset.ToSoftObjectPath();
	RenameData.NewObjectPath = FSoftObjectPath{NewObjectPath};
	RenameData.NewPackagePath = Asset.PackagePath.ToString();
	RenameData.NewName = NewName;

	if (!UGdhLibEditor::GetModuleAssetTools().Get().RenameAssets(TArray<FAssetRenameData>{RenameData}))
	{
		const FString ErrMsg = FString::Printf(TEXT("Failed To Rename %s asset"), *Asset.AssetName.ToString());
		UE_LOG(LogGdhUtil, Warning, TEXT("%s"), *ErrMsg)
		return false;
	}

	return true;
}

bool UGdhLibAsset::SaveAllAssets(const bool bPromptToUser)
{
	return FEditorFileUtils::SaveDirtyPackages(bPromptToUser, true, true, false, false, false);
}

void UGdhLibAsset::GetSourceAndConfigFiles(TSet<FString>& Files)
{
	const FString DirSrc = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir());
	const FString DirCfg = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
	const FString DirPlg = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());

	TArray<FString> SourceFiles;
	TArray<FString> ConfigFiles;

	const TSet<FString> SourceFileExtensions{TEXT("cpp"), TEXT("h"), TEXT("cs")};
	const TSet<FString> ConfigFileExtensions{TEXT("ini")};

	UGdhLibPath::GetFilesByExt(DirSrc, true, false, SourceFileExtensions, SourceFiles);
	UGdhLibPath::GetFilesByExt(DirCfg, true, false, ConfigFileExtensions, ConfigFiles);

	TArray<FString> InstalledPlugins;
	UGdhLibPath::GetFolders(DirPlg, false, InstalledPlugins);

	TArray<FString> PluginFiles;
	for (const auto& InstalledPlugin : InstalledPlugins)
	{
		UGdhLibPath::GetFilesByExt(InstalledPlugin / TEXT("Source"), true, false, SourceFileExtensions, PluginFiles);
		SourceFiles.Append(PluginFiles);

		PluginFiles.Reset();

		UGdhLibPath::GetFilesByExt(InstalledPlugin / TEXT("Config"), true, false, ConfigFileExtensions, PluginFiles);
		ConfigFiles.Append(PluginFiles);

		PluginFiles.Reset();
	}

	Files.Reset();
	Files.Append(SourceFiles);
	Files.Append(ConfigFiles);
}
