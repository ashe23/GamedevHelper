// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperAssetNamingManagerLibrary.h"
#include "GamedevHelper.h"
#include "GamedevHelperAssetLibrary.h"
#include "GamedevHelperNotificationLibrary.h"
// Engine Headers
#include "Kismet/KismetStringLibrary.h"
#include "AssetRegistryModule.h"
#include "EditorAssetLibrary.h"
#include "Misc/ScopedSlowTask.h"

void UGamedevHelperAssetNamingManagerLibrary::RenameAsset(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return;

	const auto Settings = GetDefault<UGamedevHelperAssetNamingManagerSettings>();
	if (!Settings) return;

	const FGamedevHelperRenamePreview RenamePreview = GetRenamePreview(Asset, Settings);
	if (!RenamePreview.bValid)
	{
		UGamedevHelperNotificationLibrary::ShowModal(RenamePreview.ErrMsg, EGamedevHelperModalStatus::Fail);
		return;
	}

	const bool bRenameResult = UEditorAssetLibrary::RenameAsset(RenamePreview.AssetData.ObjectPath.ToString(), RenamePreview.NewObjectPath);
	if (!bRenameResult)
	{
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Failed to rename asset"), EGamedevHelperModalStatus::Fail);
		return;
	}

	UGamedevHelperAssetLibrary::FixupRedirectors(RenamePreview.AssetData.PackagePath.ToString());
	UGamedevHelperNotificationLibrary::ShowModal(TEXT("Asset Renamed Successfully"), EGamedevHelperModalStatus::Success);
}

void UGamedevHelperAssetNamingManagerLibrary::RenameAssets(const TArray<FAssetData>& Assets)
{
	const auto Settings = GetDefault<UGamedevHelperAssetNamingManagerSettings>();
	if (!Settings) return;

	FScopedSlowTask SlowTask(
		Assets.Num(),
		FText::FromString("Renaming assets")
	);
	SlowTask.MakeDialog(true);

	bool bRenameResult = true;
	for (const auto& Asset : Assets)
	{
		SlowTask.EnterProgressFrame(
			1.0f,
			FText::FromString(Asset.AssetName.ToString())
		);

		FGamedevHelperRenamePreview RenamePreview = GetRenamePreview(Asset, Settings);
		if (RenamePreview.bValid)
		{
			UEditorAssetLibrary::RenameAsset(RenamePreview.AssetData.ObjectPath.ToString(), RenamePreview.NewObjectPath);
		}
		else
		{
			bRenameResult = false;
		}
	}

	UGamedevHelperAssetLibrary::FixupRedirectors(TEXT("/Game"));

	if (bRenameResult)
	{
		UGamedevHelperNotificationLibrary::ShowModal(TEXT("Assets Renamed Successfully"), EGamedevHelperModalStatus::Success, 3.0f);
	}
	else
	{
		UGamedevHelperNotificationLibrary::ShowModalWithOutputLog(TEXT("Failed to rename some assets"), 3.0f);
	}
}

FString UGamedevHelperAssetNamingManagerLibrary::Normalize(const FString& OriginalString)
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

FString UGamedevHelperAssetNamingManagerLibrary::Tokenize(const FString& OriginalString)
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

FString UGamedevHelperAssetNamingManagerLibrary::ConvertNamingCase(const FString& OriginalString, const EGamedevHelperNamingCase NamingCase)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	if (NamingCase == EGamedevHelperNamingCase::CamelCase)
	{
		return ConvertToCamelCase(OriginalString);
	}

	if (NamingCase == EGamedevHelperNamingCase::KebabCase)
	{
		return ConvertToKebabCase(OriginalString);
	}

	if (NamingCase == EGamedevHelperNamingCase::PascalCase)
	{
		return ConvertToPascalCase(OriginalString);
	}

	if (NamingCase == EGamedevHelperNamingCase::SnakeCase)
	{
		return ConvertToSnakeCase(OriginalString);
	}

	if (NamingCase == EGamedevHelperNamingCase::PascalSnakeCase)
	{
		return ConvertToPascalSnakeCase(OriginalString);
	}

	return FString{};
}

FString UGamedevHelperAssetNamingManagerLibrary::ConvertToPascalCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

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

FString UGamedevHelperAssetNamingManagerLibrary::ConvertToPascalSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

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

FString UGamedevHelperAssetNamingManagerLibrary::ConvertToSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	return Tokenize(OriginalString);
}

FString UGamedevHelperAssetNamingManagerLibrary::ConvertToKebabCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	return UKismetStringLibrary::JoinStringArray(Parts, TEXT("-"));
}

FString UGamedevHelperAssetNamingManagerLibrary::ConvertToCamelCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	TArray<FString> CapitalizedParts;
	CapitalizedParts.Reserve(Parts.Num());
	CapitalizedParts.Add(Parts[0]);

	for (int32 i = 1; i < Parts.Num(); ++i)
	{
		const FString FirstLetter = UKismetStringLibrary::GetSubstring(Parts[i], 0, 1).ToUpper();
		const FString RestOfStr = UKismetStringLibrary::GetSubstring(Parts[i], 1, Parts[i].Len() - 1).ToLower();
		CapitalizedParts.Add(FirstLetter + RestOfStr);
	}

	return UKismetStringLibrary::JoinStringArray(CapitalizedParts, TEXT(""));
}

FGamedevHelperRenamePreview UGamedevHelperAssetNamingManagerLibrary::GetRenamePreview(const FAssetData& Asset,
                                                                                      const UGamedevHelperAssetNamingManagerSettings* Settings)
{
	FGamedevHelperRenamePreview RenamePreview;
	RenamePreview.bValid = false;
	RenamePreview.AssetData = Asset;

	if (!Asset.IsValid())
	{
		RenamePreview.ErrMsg = TEXT("Invalid asset data");
		return RenamePreview;
	}

	if (!Settings)
	{
		RenamePreview.ErrMsg = TEXT("Invalid naming settings");
		return RenamePreview;
	}

	const FString AssetOldName = Asset.AssetName.ToString();
	const FGamedevHelperAssetNameSettings* AssetNameSettings = GetAssetNamingSettings(Asset, Settings);
	if (!AssetNameSettings)
	{
		RenamePreview.ErrMsg = FString::Printf(TEXT("Missing naming settings for %s"), *Asset.GetClass()->GetName());
		return RenamePreview;
	}

	if (AssetNameSettings->IsEmpty())
	{
		RenamePreview.ErrMsg = FString::Printf(TEXT("Prefix and Suffix are empty for %s"), *Asset.GetClass()->GetName());
		return RenamePreview;
	}

	// M_NewMaterial01 => m_new_material_01
	FString AssetBaseName = Tokenize(AssetOldName);

	// m_new_material_01 => new_material_01
	AssetBaseName.RemoveFromStart(AssetNameSettings->Prefix + TEXT("_"));
	AssetBaseName.RemoveFromEnd(TEXT("_") + AssetNameSettings->Suffix);

	const FString Prefix = AssetNameSettings->Prefix.IsEmpty() ? TEXT("") : AssetNameSettings->Prefix + TEXT("_");
	const FString Suffix = AssetNameSettings->Suffix.IsEmpty() ? TEXT("") : TEXT("_") + AssetNameSettings->Suffix;

	// new_material_01 => M_New_Material_01
	const FString FinalName = ConvertNamingCase(
		FString::Printf(
			TEXT("%s%s%s"),
			*Prefix,
			*AssetBaseName,
			*Suffix
		), EGamedevHelperNamingCase::PascalSnakeCase);

	const FAssetRegistryModule& AssetRegistry = FModuleManager::GetModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);

	// todo:ashe23 this method not working correctly for specific assets
	const FString NewAssetObjectPath = Asset.PackagePath.ToString() + FString::Printf(TEXT("/%s.%s"), *FinalName, *FinalName);
	const FAssetData OtherAsset = AssetRegistry.Get().GetAssetByObjectPath(FName{NewAssetObjectPath});
	if (OtherAsset.IsValid() && OtherAsset.AssetName.ToString().Equals(FinalName, ESearchCase::CaseSensitive))
	{
		RenamePreview.ErrMsg = TEXT("Asset with same name already exists in directory");
		return RenamePreview;
	}

	RenamePreview.bValid = true;
	RenamePreview.NewName = FinalName;
	RenamePreview.NewObjectPath = NewAssetObjectPath;

	return RenamePreview;
}

const FGamedevHelperAssetNameSettings* UGamedevHelperAssetNamingManagerLibrary::GetAssetNamingSettings(
	const FAssetData& Asset,
	const UGamedevHelperAssetNamingManagerSettings* Settings)
{
	if (!Asset.IsValid()) return nullptr;
	if (!Settings) return nullptr;

	// todo:ashe23 some assets require additional searching methods in order to find suffix correctly (niagara scripts, blueprints etc)
	const auto AssetNaming = Settings->FindNamingByClass(Asset.GetClass());
	if (AssetNaming)
	{
		return AssetNaming;
	}

	if (UGamedevHelperAssetLibrary::IsBlueprint(Asset))
	{
		return Settings->BlueprintTypesNaming.Find(UGamedevHelperAssetLibrary::GetBlueprintType(Asset));
	}

	return nullptr;
}
