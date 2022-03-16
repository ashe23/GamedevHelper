// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperAssetNamingManagerLibrary.h"
#include "GamedevHelperStringLibrary.h"
// Engine Headers
#include "Kismet/KismetStringLibrary.h"

void UGamedevHelperAssetNamingManagerLibrary::RenameAsset(const FAssetData& Asset, const UGamedevHelperAssetNamingManagerSettings* Settings)
{
	// todo:ashe23
}

void UGamedevHelperAssetNamingManagerLibrary::RenameAssets(const TArray<FAssetData>& Assets, const UGamedevHelperAssetNamingManagerSettings* Settings)
{
	// todo:ashe23
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
	
	for(int32 i = 1; i < Chars.Num() - 1; ++i)
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
			Tokens.Add(Token.ToLower());
			Token.Empty();
		}
		
		Token.AppendChar(CurrentChar);
	}

	Tokens.Add(Token.ToLower());
	
	return UKismetStringLibrary::JoinStringArray(Tokens, TEXT("_"));
}

FString UGamedevHelperAssetNamingManagerLibrary::ConvertNamingCase(const FString& OriginalString, const EGamedevHelperNamingCase NamingCase)
{
	// todo:ashe23
	return FString{};
}
