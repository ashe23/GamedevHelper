// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSubsystem.h"
#include "Gdh.h"
#include "Settings/GdhAssetNamingConvention.h"
// Engine Headers
#include "IContentBrowserSingleton.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Kismet/KismetStringLibrary.h"
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

	const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	if (!NamingConvention) return {};

	const FString OldName = InAssetData.AssetName.ToString();
	const FString TokenizedName = Tokenize(OldName);
	const FString BaseNameWithoutPrefixAndSuffix = RemoveOldPrefixAndSuffix(TokenizedName, NamingConvention);
	// const FString Prefix = NamingInfo.Prefix.IsEmpty() ? TEXT("") : NamingInfo.Prefix + TEXT("_");
	// const FString Suffix = NamingInfo.Suffix.IsEmpty() ? TEXT("") : TEXT("_") + NamingInfo.Suffix;

	FString FinalName;

	if (NamingConvention->bApplyNamingCaseOnPrefixAndSuffix)
	{
		const FString Prefix = NamingInfo.Prefix.IsEmpty() ? TEXT("") : NamingInfo.Prefix + TEXT("_");
		const FString Suffix = NamingInfo.Suffix.IsEmpty() ? TEXT("") : TEXT("_") + NamingInfo.Suffix;

		FinalName = FString::Printf(
			TEXT("%s%s%s"),
			Prefix.IsEmpty() ? TEXT("") : *(ConvertNamingCase(Prefix, NamingConvention->NamingCase) + TEXT("_")),
			*ConvertNamingCase(BaseNameWithoutPrefixAndSuffix, NamingConvention->NamingCase),
			Suffix.IsEmpty() ? TEXT("") : *(TEXT("_") + ConvertNamingCase(Suffix, NamingConvention->NamingCase))
		);
	}
	else
	{
		const FString Prefix = NamingInfo.Prefix.IsEmpty() ? TEXT("") : NamingInfo.Prefix + TEXT("_");
		const FString Suffix = NamingInfo.Suffix.IsEmpty() ? TEXT("") : TEXT("_") + NamingInfo.Suffix;

		FinalName = Prefix + ConvertNamingCase(BaseNameWithoutPrefixAndSuffix, NamingConvention->NamingCase) + Suffix;
	}

	return FinalName;
}

FGdhAssetNamingInfo UGdhSubsystem::GetAssetNamingInfoByAsset(const FAssetData& InAssetData)
{
	if (!InAssetData.IsValid()) return {};

	const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	if (!NamingConvention) return {};

	if (!InAssetData.AssetClass.IsEqual(TEXT("Blueprint")))
	{
		return GetAssetNamingInfoByAssetClass(InAssetData.GetClass());
	}

	const UClass* ParentClass = GetBlueprintParentClass(InAssetData);
	const UClass* SearchClass = ParentClass ? ParentClass : UBlueprint::StaticClass();
	FGdhAssetNamingInfo NameFormat = GetAssetNamingInfoByAssetClass(SearchClass);

	if (NameFormat.Prefix.IsEmpty())
	{
		const auto BlueprintType = GetBlueprintType(InAssetData);
		const auto BlueprintTypePrefix = NamingConvention->BlueprintPrefixes.Find(BlueprintType);
		const FString BlueprintPrefix = BlueprintTypePrefix ? *BlueprintTypePrefix->Prefix : TEXT("BP");
		NameFormat.Prefix = BlueprintPrefix;
	}

	return NameFormat;
}

FGdhAssetNamingInfo UGdhSubsystem::GetAssetNamingInfoByAssetClass(const UClass* InAssetClass)
{
	if (!InAssetClass) return {};

	const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	if (!NamingConvention) return {};

	for (const auto& Naming : NamingConvention->Mappings)
	{
		const UClass* NamingClass = Naming.Key;
		if (NamingClass == InAssetClass)
		{
			return Naming.Value;
		}
	}

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

	if (NamingCase == EGdhNamingCase::SnakeCase)
	{
		return ConvertToSnakeCase(OriginalString);
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

	const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	if (!NamingConvention) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	TArray<FString> CapitalizedParts;
	CapitalizedParts.Reserve(Parts.Num());

	for (const auto& Part : Parts)
	{
		if (NamingConvention->ReservedKeywords.Contains(Part))
		{
			CapitalizedParts.Add(Part);
		}
		else
		{
			const FString FirstLetter = UKismetStringLibrary::GetSubstring(Part, 0, 1).ToUpper();
			const FString RestOfStr = UKismetStringLibrary::GetSubstring(Part, 1, Part.Len() - 1).ToLower();
			CapitalizedParts.Add(FirstLetter + RestOfStr);
		}
	}

	return UKismetStringLibrary::JoinStringArray(CapitalizedParts, TEXT(""));
}

FString UGdhSubsystem::ConvertToPascalSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const UGdhAssetNamingConvention* NamingConvention = GetDefault<UGdhAssetNamingConvention>();
	if (!NamingConvention) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	TArray<FString> CapitalizedParts;
	CapitalizedParts.Reserve(Parts.Num());

	for (const auto& Part : Parts)
	{
		if (NamingConvention->ReservedKeywords.Contains(Part))
		{
			CapitalizedParts.Add(Part);
		}
		else
		{
			const FString FirstLetter = UKismetStringLibrary::GetSubstring(Part, 0, 1).ToUpper();
			const FString RestOfStr = UKismetStringLibrary::GetSubstring(Part, 1, Part.Len() - 1).ToLower();
			CapitalizedParts.Add(FirstLetter + RestOfStr);
		}
	}

	return UKismetStringLibrary::JoinStringArray(CapitalizedParts, TEXT("_"));
}

FString UGdhSubsystem::ConvertToSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	return Tokenize(OriginalString);
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

FString UGdhSubsystem::RemoveOldPrefixAndSuffix(const FString& OldAssetName, const UGdhAssetNamingConvention* NamingConvention)
{
	if (OldAssetName.IsEmpty()) return OldAssetName;
	if (!NamingConvention) return OldAssetName;

	const auto NamingManagerSettings = GetDefault<UGdhAssetNamingConvention>();
	if (!NamingManagerSettings)
	{
		UE_LOG(LogGdh, Error, TEXT("Invalid Asset Naming Manager Settings"));
		return OldAssetName;
	}

	FString BaseName = OldAssetName;

	for (const auto& OldPrefix : NamingManagerSettings->OldPrefixes)
	{
		if (OldPrefix.IsEmpty()) continue;

		BaseName.RemoveFromStart(OldPrefix + TEXT("_"));
	}

	for (const auto& OldSuffix : NamingManagerSettings->OldSuffixes)
	{
		if (OldSuffix.IsEmpty()) continue;

		BaseName.RemoveFromEnd(TEXT("_") + OldSuffix);
	}

	for (const auto& Naming : NamingConvention->Mappings)
	{
		if (Naming.Key)
		{
			if (!Naming.Value.Prefix.IsEmpty())
			{
				BaseName.RemoveFromStart(Naming.Value.Prefix + TEXT("_"));
			}

			if (!Naming.Value.Suffix.IsEmpty())
			{
				BaseName.RemoveFromEnd(TEXT("_") + Naming.Value.Suffix);
			}
		}
	}

	return BaseName;
}
