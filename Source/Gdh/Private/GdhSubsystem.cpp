// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSubsystem.h"
#include "Gdh.h"
// Engine Headers
#include "IContentBrowserSingleton.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Internationalization/Regex.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/ScopedSlowTask.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

#if WITH_EDITOR
void UGdhSubsystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif

void UGdhSubsystem::GetAssetsAll(TArray<FAssetData>& OutAssets)
{
	if (GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	OutAssets.Reset();

	GetModuleAssetRegistry().Get().GetAssetsByPath(GdhConstants::PathRoot, OutAssets, true);
}

void UGdhSubsystem::GetAssetsIndirect(TArray<FAssetData>& OutAssets, TArray<FGdhAssetIndirectInfo>& AssetIndirectInfos, const bool bShowSlowTask)
{
	if (GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	OutAssets.Reset();
	AssetIndirectInfos.Reset();

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

			const FString ObjectPath = PathConvertToObjectPath(FoundedAssetObjectPath);
			if (ObjectPath.IsEmpty()) continue;

			const FAssetData AssetData = GetModuleAssetRegistry().Get().GetAssetByObjectPath(FName{*ObjectPath});
			if (!AssetData.IsValid()) continue;

			// if founded asset is ok, we loading file lines to determine on what line its used
			TArray<FString> Lines;
			FFileHelper::LoadFileToStringArray(Lines, *File);

			for (int32 i = 0; i < Lines.Num(); ++i)
			{
				if (!Lines.IsValidIndex(i)) continue;
				if (!Lines[i].Contains(FoundedAssetObjectPath)) continue;

				const FString FilePathAbs = FPaths::ConvertRelativePathToFull(File);
				const int32 FileLine = i + 1;

				AssetIndirectInfos.AddUnique(FGdhAssetIndirectInfo{AssetData, FilePathAbs, FileLine});
				OutAssets.AddUnique(AssetData);
			}
		}
	}
}

void UGdhSubsystem::GetAssetsByPath(const FString& InPath, const bool bRecursive, TArray<FAssetData>& OutAssets)
{
	if (GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	OutAssets.Reset();

	GetModuleAssetRegistry().Get().GetAssetsByPath(FName{*InPath}, OutAssets, bRecursive);
}

FAssetData UGdhSubsystem::GetAssetByObjectPath(const FString& InPath)
{
	if (GetModuleAssetRegistry().Get().IsLoadingAssets()) return {};

	return GetModuleAssetRegistry().Get().GetAssetByObjectPath(FName{*InPath});
}

void UGdhSubsystem::GetProjectRedirectors(TArray<FAssetData>& Redirectors)
{
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace(GdhConstants::PathRoot);
	Filter.ClassNames.Emplace(UObjectRedirector::StaticClass()->GetFName());

	Redirectors.Reset();
	GetModuleAssetRegistry().Get().GetAssets(Filter, Redirectors);
}

bool UGdhSubsystem::ProjectHasRedirectors()
{
	TArray<FAssetData> Redirectors;
	GetProjectRedirectors(Redirectors);

	return Redirectors.Num() > 0;
}

void UGdhSubsystem::FixProjectRedirectors(const TArray<FAssetData>& Redirectors, const bool bShowSlowTask)
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

	GetModuleAssetTools().Get().FixupReferencers(RedirectorObjects, false);
}

FString UGdhSubsystem::GetAssetRenamePreview(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid()) return {};

	const FGdhAssetNamingInfo NamingInfo = GetAssetNamingInfoByAsset(InAssetData);
	if (NamingInfo.Prefix.IsEmpty() && NamingInfo.Suffix.IsEmpty()) return {};

	// const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	// if (!NamingConvention) return {};

	const FString OldName = InAssetData.AssetName.ToString();
	const FString TokenizedName = Tokenize(OldName);
	const FString BaseNameWithoutPrefixAndSuffix = RemoveOldPrefixAndSuffix(TokenizedName);
	const FString Prefix = NamingInfo.Prefix.IsEmpty() ? TEXT("") : NamingInfo.Prefix + TEXT("_");
	const FString Suffix = NamingInfo.Suffix.IsEmpty() ? TEXT("") : TEXT("_") + NamingInfo.Suffix;

	return {};
	// return Prefix + ConvertNamingCase(BaseNameWithoutPrefixAndSuffix, NamingConvention->NamingCase) + Suffix;
}

FGdhAssetNamingInfo UGdhSubsystem::GetAssetNamingInfoByAsset(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid()) return {};

	// const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	// if (!NamingConvention) return {};

	if (!InAssetData.AssetClass.IsEqual(TEXT("Blueprint")))
	{
		return GetAssetNamingInfoByAssetClass(InAssetData.GetClass());
	}

	const UClass* ParentClass = GetBlueprintParentClass(InAssetData);
	const UClass* SearchClass = ParentClass ? ParentClass : UBlueprint::StaticClass();
	// const EGdhBlueprintType BlueprintType = GetBlueprintType(InAssetData);
	const FGdhAssetNamingInfo NamingInfo = GetAssetNamingInfoByAssetClass(SearchClass);
	// const FGdhAssetNamingInfo* BlueprintNamingInfo = NamingConvention->BlueprintPrefixes.Find(BlueprintType);

	// if (NamingInfo.Prefix.IsEmpty() && BlueprintNamingInfo && BlueprintNamingInfo->Prefix.IsEmpty()) return {};
	//
	// if (BlueprintNamingInfo && (BlueprintType != EGdhBlueprintType::None && BlueprintType != EGdhBlueprintType::Normal))
	// {
	// 	return FGdhAssetNamingInfo{BlueprintNamingInfo->Prefix, NamingInfo.Suffix};
	// }

	return NamingInfo;
}

FGdhAssetNamingInfo UGdhSubsystem::GetAssetNamingInfoByAssetClass(const UClass* InAssetClass)
{
	if (!InAssetClass) return {};

	// const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	// if (!NamingConvention) return {};
	//
	// for (const auto& Naming : NamingConvention->Mappings)
	// {
	// 	const UClass* NamingClass = Naming.Key;
	// 	if (NamingClass == InAssetClass)
	// 	{
	// 		return Naming.Value;
	// 	}
	// }

	return {};
}

EGdhBlueprintType UGdhSubsystem::GetBlueprintType(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid())
	{
		return EGdhBlueprintType::None;
	}

	FString BlueprintTypeStr;
	InAssetData.GetTagValue(TEXT("BlueprintType"), BlueprintTypeStr);

	FString ParentClassStr;
	InAssetData.GetTagValue(TEXT("ParentClass"), ParentClassStr);

	if (BlueprintTypeStr.IsEmpty())
	{
		return EGdhBlueprintType::None;
	}

	const EBlueprintType BlueprintType = GdhConstants::GetEnumValueFromString<EBlueprintType>(BlueprintTypeStr);

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

UClass* UGdhSubsystem::GetBlueprintParentClass(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid()) return nullptr;

	const auto BlueprintAsset = Cast<UBlueprint>(InAssetData.GetAsset());
	if (!BlueprintAsset) return nullptr;

	return BlueprintAsset->ParentClass;
}

void UGdhSubsystem::OpenAssetEditor(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid()) return;
	if (!GEditor) return;

	TArray<FName> AssetNames;
	AssetNames.Add(InAssetData.ToSoftObjectPath().GetAssetPathName());

	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorsForAssets(AssetNames);
}

void UGdhSubsystem::OpenAssetInContentBrowser(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid()) return;

	GetModuleContentBrowser().Get().FocusPrimaryContentBrowser(true);
	GetModuleContentBrowser().Get().SyncBrowserToAssets(TArray<FAssetData>{InAssetData});
}

FString UGdhSubsystem::Normalize(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	FString CleanedString;
	CleanedString.Reserve(OriginalString.Len());

	const auto Chars = OriginalString.GetCharArray();
	for (const auto Char : Chars)
	{
		if (FChar::IsAlnum(Char) || FChar::IsDigit(Char))
		{
			CleanedString.AppendChar(Char);
		}
		else
		{
			CleanedString.AppendChar('_');
		}
	}

	TArray<FString> Parts;
	CleanedString.ParseIntoArray(Parts, TEXT("_"), true);

	return UKismetStringLibrary::JoinStringArray(Parts, TEXT("_"));
}

FString UGdhSubsystem::Tokenize(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Normalized = Normalize(OriginalString);

	FString Token;
	Token.AppendChar(Normalized[0]);
	const auto Chars = Normalized.GetCharArray();

	TArray<FString> Tokens;

	for (int32 i = 1; i < Chars.Num() - 1; ++i)
	{
		const auto CurrentChar = Chars[i];
		const auto PreviousChar = Chars[i - 1];

		if (FChar::IsUnderscore(CurrentChar))
		{
			continue;
		}

		if (
			!FChar::IsUnderscore(CurrentChar) && FChar::IsUnderscore(PreviousChar) ||
			FChar::IsLower(CurrentChar) && FChar::IsDigit(PreviousChar) ||
			FChar::IsUpper(CurrentChar) && FChar::IsLower(PreviousChar) ||
			FChar::IsUpper(CurrentChar) && FChar::IsDigit(PreviousChar) ||
			FChar::IsDigit(CurrentChar) && FChar::IsLower(PreviousChar) ||
			FChar::IsDigit(CurrentChar) && FChar::IsUpper(PreviousChar)
		)
		{
			Tokens.Add(Token);
			Token.Empty();
		}

		Token.AppendChar(CurrentChar);
	}

	Tokens.Add(Token);

	return UKismetStringLibrary::JoinStringArray(Tokens, TEXT("_")).ToLower();
}

FString UGdhSubsystem::ConvertNamingCase(const FString& OriginalString, const EGdhNamingCase NamingCase)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	if (NamingCase == EGdhNamingCase::PascalCase)
	{
		return ConvertToPascalCase(OriginalString);
	}

	if (NamingCase == EGdhNamingCase::PascalSnakeCase)
	{
		return ConvertToPascalSnakeCase(OriginalString);
	}

	return FString{};
}

FString UGdhSubsystem::ConvertToPascalCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	// const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	// if (!NamingConvention) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	TArray<FString> CapitalizedParts;
	CapitalizedParts.Reserve(Parts.Num());

	for (const auto& Part : Parts)
	{
		const FString FirstLetter = UKismetStringLibrary::GetSubstring(Part, 0, 1).ToUpper();
		const FString RestOfStr = UKismetStringLibrary::GetSubstring(Part, 1, Part.Len() - 1).ToLower();
		CapitalizedParts.Add(FirstLetter + RestOfStr);
	}

	return UKismetStringLibrary::JoinStringArray(CapitalizedParts, TEXT(""));
}

FString UGdhSubsystem::ConvertToPascalSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	// const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	// if (!NamingConvention) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	TArray<FString> CapitalizedParts;
	CapitalizedParts.Reserve(Parts.Num());

	for (const auto& Part : Parts)
	{
		const FString FirstLetter = UKismetStringLibrary::GetSubstring(Part, 0, 1).ToUpper();
		const FString RestOfStr = UKismetStringLibrary::GetSubstring(Part, 1, Part.Len() - 1).ToLower();
		CapitalizedParts.Add(FirstLetter + RestOfStr);
	}

	return UKismetStringLibrary::JoinStringArray(CapitalizedParts, TEXT("_"));
}

FString UGdhSubsystem::PathNormalize(const FString& InPath)
{
	if (InPath.IsEmpty()) return {};

	// Ensure the path dont starts with a slash or a disk drive letter
	if (!(InPath.StartsWith(TEXT("/")) || InPath.StartsWith(TEXT("\\")) || (InPath.Len() > 2 && InPath[1] == ':')))
	{
		return {};
	}

	FString Path = FPaths::ConvertRelativePathToFull(InPath).TrimStartAndEnd();
	FPaths::RemoveDuplicateSlashes(Path);

	// Collapse any ".." or "." references in the path
	FPaths::CollapseRelativeDirectories(Path);

	if (FPaths::GetExtension(Path).IsEmpty())
	{
		FPaths::NormalizeDirectoryName(Path);
	}
	else
	{
		FPaths::NormalizeFilename(Path);
	}

	// Ensure the path does not end with a trailing slash
	if (Path.EndsWith(TEXT("/")) || Path.EndsWith(TEXT("\\")))
	{
		Path = Path.LeftChop(1);
	}

	return Path;
}

FString UGdhSubsystem::PathConvertToObjectPath(const FString& InPath)
{
	if (FPaths::FileExists(InPath))
	{
		const FString FileName = FPaths::GetBaseFilename(InPath);
		const FString AssetPath = PathConvertToRelative(FPaths::GetPath(InPath));

		return FString::Printf(TEXT("%s/%s.%s"), *AssetPath, *FileName, *FileName);
	}

	FString ObjectPath = FPackageName::ExportTextPathToObjectPath(InPath);
	ObjectPath.RemoveFromEnd(TEXT("_C")); // we should remove _C prefix if its blueprint asset

	if (!ObjectPath.StartsWith(GdhConstants::PathRoot.ToString())) return {};

	TArray<FString> Parts;
	ObjectPath.ParseIntoArray(Parts, TEXT("/"), true);

	if (Parts.Num() > 0)
	{
		FString Left;
		FString Right;
		Parts.Last().Split(TEXT("."), &Left, &Right);

		if (!Left.IsEmpty() && !Right.IsEmpty() && Left.Equals(*Right))
		{
			return ObjectPath;
		}
	}

	return {};
}

FString UGdhSubsystem::PathConvertToRelative(const FString& InPath)
{
	const FString PathNormalized = PathNormalize(InPath);
	const FString PathProjectContent = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()).LeftChop(1);

	if (PathNormalized.IsEmpty()) return {};
	if (PathNormalized.StartsWith(GdhConstants::PathRoot.ToString())) return PathNormalized;
	if (PathNormalized.StartsWith(PathProjectContent))
	{
		FString Path = PathNormalized;
		Path.RemoveFromStart(PathProjectContent);

		return Path.IsEmpty() ? GdhConstants::PathRoot.ToString() : GdhConstants::PathRoot.ToString() / Path;
	}

	return {};
}

void UGdhSubsystem::GetFolders(const FString& InSearchPath, const bool bSearchRecursive, TArray<FString>& OutFolders)
{
	OutFolders.Empty();

	struct FFindFoldersVisitor : IPlatformFile::FDirectoryVisitor
	{
		TArray<FString>& Folders;

		explicit FFindFoldersVisitor(TArray<FString>& InFolders) : Folders(InFolders) { }

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory)
			{
				Folders.Emplace(FPaths::ConvertRelativePathToFull(FilenameOrDirectory));
			}

			return true;
		}
	};

	FFindFoldersVisitor FindFoldersVisitor{OutFolders};
	if (bSearchRecursive)
	{
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectoryRecursively(*InSearchPath, FindFoldersVisitor);
	}
	else
	{
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*InSearchPath, FindFoldersVisitor);
	}
}

void UGdhSubsystem::GetFilesByExt(const FString& InSearchPath, const bool bSearchRecursive, const bool bExtSearchInvert, const TSet<FString>& InExtensions, TArray<FString>& OutFiles)
{
	OutFiles.Empty();

	struct FFindFilesVisitor : IPlatformFile::FDirectoryVisitor
	{
		const bool bSearchInvert;
		TArray<FString>& Files;
		const TSet<FString>& Extensions;

		explicit FFindFilesVisitor(const bool bInSearchInvert, TArray<FString>& InFiles, const TSet<FString>& InExtensions)
			: bSearchInvert(bInSearchInvert),
			  Files(InFiles),
			  Extensions(InExtensions) { }

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (!bIsDirectory)
			{
				const FString FullPath = FPaths::ConvertRelativePathToFull(FilenameOrDirectory);

				if (Extensions.Num() == 0)
				{
					Files.Emplace(FullPath);
					return true;
				}

				const FString Ext = FPaths::GetExtension(FullPath, false);
				const bool bExistsInSearchList = Extensions.Contains(Ext);

				if (
					bExistsInSearchList && !bSearchInvert ||
					!bExistsInSearchList && bSearchInvert
				)
				{
					Files.Emplace(FullPath);
				}
			}

			return true;
		}
	};

	TSet<FString> ExtensionsNormalized;
	ExtensionsNormalized.Reserve(InExtensions.Num());

	for (const auto& Ext : InExtensions)
	{
		const FString ExtNormalized = Ext.Replace(TEXT("."), TEXT(""));
		ExtensionsNormalized.Emplace(ExtNormalized);
	}

	FFindFilesVisitor FindFilesVisitor{bExtSearchInvert, OutFiles, ExtensionsNormalized};
	if (bSearchRecursive)
	{
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectoryRecursively(*InSearchPath, FindFilesVisitor);
	}
	else
	{
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*InSearchPath, FindFilesVisitor);
	}
}

void UGdhSubsystem::GetSourceAndConfigFiles(TSet<FString>& OutFiles)
{
	const FString DirSrc = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir());
	const FString DirCfg = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
	const FString DirPlg = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());

	TArray<FString> SourceFiles;
	TArray<FString> ConfigFiles;

	GetFilesByExt(DirSrc, true, false, GdhConstants::SourceFileExtensions, SourceFiles);
	GetFilesByExt(DirCfg, true, false, GdhConstants::ConfigFileExtensions, ConfigFiles);

	TArray<FString> InstalledPlugins;
	GetFolders(DirPlg, false, InstalledPlugins);

	const FString ProjectCleanerPluginPath = DirPlg / GdhConstants::ModuleName.ToString();
	TArray<FString> PluginFiles;
	for (const auto& InstalledPlugin : InstalledPlugins)
	{
		// ignore ProjectCleaner plugin
		if (InstalledPlugin.Equals(ProjectCleanerPluginPath)) continue;

		GetFilesByExt(InstalledPlugin / TEXT("Source"), true, false, GdhConstants::SourceFileExtensions, PluginFiles);
		SourceFiles.Append(PluginFiles);

		PluginFiles.Reset();

		GetFilesByExt(InstalledPlugin / TEXT("Config"), true, false, GdhConstants::ConfigFileExtensions, PluginFiles);
		ConfigFiles.Append(PluginFiles);

		PluginFiles.Reset();
	}

	OutFiles.Reset();
	OutFiles.Append(SourceFiles);
	OutFiles.Append(ConfigFiles);
}

void UGdhSubsystem::ShowNotification(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.Text = FText::FromString(Msg);
	Info.ExpireDuration = Duration;

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(State);
}

void UGdhSubsystem::ShowNotificationWithOutputLog(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.Text = FText::FromString(Msg);
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateLambda([]()
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FName{TEXT("OutputLog")});
	});
	Info.HyperlinkText = FText::FromString(TEXT("Show OutputLog..."));

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(State);
}

FAssetRegistryModule& UGdhSubsystem::GetModuleAssetRegistry()
{
	return FModuleManager::LoadModuleChecked<FAssetRegistryModule>(GdhConstants::ModuleAssetRegistry);
}

FPropertyEditorModule& UGdhSubsystem::GetModulePropertyEditor()
{
	return FModuleManager::LoadModuleChecked<FPropertyEditorModule>(GdhConstants::ModulePropertyEditor);
}

FAssetToolsModule& UGdhSubsystem::GetModuleAssetTools()
{
	return FModuleManager::LoadModuleChecked<FAssetToolsModule>(GdhConstants::ModuleAssetTools);
}

FContentBrowserModule& UGdhSubsystem::GetModuleContentBrowser()
{
	return FModuleManager::LoadModuleChecked<FContentBrowserModule>(GdhConstants::ModuleContentBrowser);
}

FString UGdhSubsystem::RemoveOldPrefixAndSuffix(const FString& OldAssetName)
{
	if (OldAssetName.IsEmpty()) return OldAssetName;

	// const auto NamingManagerSettings = GetDefault<UGdhAssetNamingConvention>();
	// if (!NamingManagerSettings)
	// {
	// 	UE_LOG(LogGdh, Error, TEXT("Invalid Asset Naming Manager Settings"));
	// 	return OldAssetName;
	// }

	FString BaseName = OldAssetName;
	//
	// for (const auto& OldPrefix : NamingManagerSettings->OldPrefixes)
	// {
	// 	if (OldPrefix.IsEmpty()) continue;
	//
	// 	BaseName.RemoveFromStart(OldPrefix + TEXT("_"));
	// }
	//
	// for (const auto& OldSuffix : NamingManagerSettings->OldSuffixes)
	// {
	// 	if (OldSuffix.IsEmpty()) continue;
	//
	// 	BaseName.RemoveFromEnd(TEXT("_") + OldSuffix);
	// }
	//
	// for (const auto& Naming : NamingManagerSettings->Mappings)
	// {
	// 	if (Naming.Key)
	// 	{
	// 		if (!Naming.Value.Prefix.IsEmpty())
	// 		{
	// 			BaseName.RemoveFromStart(Naming.Value.Prefix + TEXT("_"));
	// 		}
	//
	// 		if (!Naming.Value.Suffix.IsEmpty())
	// 		{
	// 			BaseName.RemoveFromEnd(TEXT("_") + Naming.Value.Suffix);
	// 		}
	// 	}
	// }
	//
	// for (const auto& Naming : NamingManagerSettings->BlueprintPrefixes)
	// {
	// 	if (!Naming.Value.Prefix.IsEmpty())
	// 	{
	// 		BaseName.RemoveFromStart(Naming.Value.Prefix + TEXT("_"));
	// 	}
	//
	// 	if (!Naming.Value.Suffix.IsEmpty())
	// 	{
	// 		BaseName.RemoveFromEnd(TEXT("_") + Naming.Value.Suffix);
	// 	}
	// }

	return BaseName;
}
