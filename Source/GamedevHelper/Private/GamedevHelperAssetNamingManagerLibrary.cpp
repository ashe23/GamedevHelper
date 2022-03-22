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

void UGamedevHelperAssetNamingManagerLibrary::RenameAssets(const TArray<FAssetData>& Assets)
{
	const auto NamingConvention = GetDefault<UGamedevHelperAssetNamingConvention>();
	if (!NamingConvention) return;

	bool bRenameResult = true;

	TArray<FGamedevHelperRenamePreview> Previews;
	GetRenamePreviews(Assets, NamingConvention, Previews);

	if (Previews.Num() == 0)
	{
		bRenameResult = false;
	}

	FScopedSlowTask SlowTask(
		Assets.Num(),
		FText::FromString("Renaming assets")
	);
	SlowTask.MakeDialog(true);

	for (const auto& Preview : Previews)
	{
		SlowTask.EnterProgressFrame(
			1.0f,
			FText::FromString(Preview.GetNewName())
		);

		if (!(Preview.IsValid() && UEditorAssetLibrary::RenameAsset(Preview.GetAssetData().ObjectPath.ToString(), Preview.GetNewObjectPath())))
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

void UGamedevHelperAssetNamingManagerLibrary::GetRenamePreviews(const TArray<FAssetData>& Assets,
                                                                const UGamedevHelperAssetNamingConvention* NamingConvention,
                                                                TArray<FGamedevHelperRenamePreview>& Previews)
{
	if (!NamingConvention)
	{
		UE_LOG(LogGamedevHelper, Error, TEXT("Invalid Asset Naming Convention object"));
		return;
	}

	Previews.Reset();
	Previews.Reserve(Assets.Num());

	for (const auto& Asset : Assets)
	{
		FGamedevHelperRenamePreview RenamePreview;
		RenamePreview.SetAssetData(Asset);
		if (!RenamePreview.GetAssetData().IsValid()) continue;

		const FGamedevHelperAssetNameFormat NamingFormat = NamingConvention->GetAssetNameFormat(Asset);
		if (!NamingFormat.IsValid())
		{
			RenamePreview.SetStatus(EGamedevHelperRenameStatus::MissingSettings);
			Previews.Add(RenamePreview);
			continue;
		}

		const FString OldName = Asset.AssetName.ToString();

		// {OldPrefix}_{BaseName}_{OldSuffix} => {BaseName}
		FString BaseName = Normalize(OldName);
		BaseName.RemoveFromStart(NamingFormat.Prefix + TEXT("_"));
		BaseName.RemoveFromEnd(TEXT("_") + NamingFormat.Suffix);

		const FString Prefix = NamingFormat.Prefix.IsEmpty() ? TEXT("") : NamingFormat.Prefix + TEXT("_");
		const FString Suffix = NamingFormat.Suffix.IsEmpty() ? TEXT("") : TEXT("_") + NamingFormat.Suffix;
		const FString NewName = Prefix + ConvertNamingCase(BaseName, EGamedevHelperNamingCase::PascalCase) + Suffix;
		const FString NewObjectPath = Asset.PackagePath.ToString() + FString::Printf(TEXT("/%s.%s"), *NewName, *NewName);
		RenamePreview.SetNewName(NewName);
		RenamePreview.SetNewObjectPath(NewObjectPath);
		
		const auto OtherPreviewWithSameName = Previews.FindByPredicate([&](const FGamedevHelperRenamePreview& OtherPrev)
		{
			return
				OtherPrev.GetNewName().Equals(NewName, ESearchCase::CaseSensitive) &&
				OtherPrev.GetAssetData().ObjectPath.ToString().Equals(NewObjectPath);
		});

		if (UEditorAssetLibrary::DoesAssetExist(NewObjectPath) && !OldName.Equals(NewName, ESearchCase::CaseSensitive))
		{
			RenamePreview.SetStatus(EGamedevHelperRenameStatus::DuplicateNameContentBrowser);
		}
		else if (!UEditorAssetLibrary::DoesAssetExist(NewObjectPath) && !OldName.Equals(NewName, ESearchCase::CaseSensitive))
		{
			RenamePreview.SetStatus(EGamedevHelperRenameStatus::OkToRename);
		}
		else if (OtherPreviewWithSameName)
		{
			OtherPreviewWithSameName->SetStatus(EGamedevHelperRenameStatus::DuplicateNamePreview);
			RenamePreview.SetStatus(EGamedevHelperRenameStatus::DuplicateNamePreview);
		}

		Previews.Add(RenamePreview);
	}
}