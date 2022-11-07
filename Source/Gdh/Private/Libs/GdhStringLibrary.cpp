// Copyright Ashot Barkhudaryan All Rights Reserved.

#include "Libs/GdhStringLibrary.h"
// Engine Headers
#include "Gdh.h"
#include "Kismet/KismetStringLibrary.h"

FString UGdhStringLibrary::GetRandomStringFromCharset(const int32 Len, const FString& Charset, const int32 Seed)
{
	if (Len <= 0 || Charset.IsEmpty())
	{
		return FString{};
	}

	const FRandomStream RandomStream{Seed};
	const auto Chars = Charset.GetCharArray();

	FString FinalString;
	FinalString.Reserve(Len);

	for (int32 Index = 0; Index < Len; ++Index)
	{
		const int32 RandIndex = RandomStream.RandRange(0, Chars.Num() - 2); // ignore '\0' element
		FinalString.AppendChar(Chars[RandIndex]);
	}

	return FinalString;
}

FString UGdhStringLibrary::GetFirstLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, 0, 1);
}

FString UGdhStringLibrary::GetLastLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, OriginalString.Len() - 1, 1);
}

FString UGdhStringLibrary::RemoveFirstLetter(const FString& OriginalString)
{
	return OriginalString.RightChop(1);
}

FString UGdhStringLibrary::RemoveLastLetter(const FString& OriginalString)
{
	return OriginalString.LeftChop(1);
}

FString UGdhStringLibrary::Intersection(const FString& StringA, const FString& StringB)
{
	if (StringA.IsEmpty() || StringB.IsEmpty())
	{
		return FString{};
	}

	FString Chars;
	Chars.Reserve(StringA.Len() + StringB.Len());

	for (int32 i = 0; i < StringA.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);

		if (StringB.Contains(SingleChar, ESearchCase::CaseSensitive))
		{
			Chars.Append(SingleChar);
		}
	}

	Chars.Shrink();

	return Chars;
}

FString UGdhStringLibrary::Union(const FString& StringA, const FString& StringB)
{
	FString Chars;
	Chars.Reserve(StringA.Len() + StringB.Len());

	for (int32 i = 0; i < StringA.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
		if (!Chars.Contains(SingleChar, ESearchCase::CaseSensitive))
		{
			Chars.Append(SingleChar);
		}
	}

	for (int32 i = 0; i < StringB.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(StringB, i, 1);
		if (!Chars.Contains(SingleChar, ESearchCase::CaseSensitive))
		{
			Chars.Append(SingleChar);
		}
	}

	Chars.Shrink();

	return Chars;
}

FString UGdhStringLibrary::SymmetricDifference(const FString& StringA, const FString& StringB)
{
	// Symmetric difference = union - intersection
	const FString UnionSet = Union(StringA, StringB);
	const FString IntersectionSet = Intersection(StringA, StringB);

	FString Chars;
	Chars.Reserve(UnionSet.Len());

	for (int32 i = 0; i < UnionSet.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(UnionSet, i, 1);
		if (!IntersectionSet.Contains(SingleChar, ESearchCase::CaseSensitive))
		{
			Chars.Append(SingleChar);
		}
	}

	Chars.Shrink();

	return Chars;
}

FString UGdhStringLibrary::Difference(const FString& StringA, const FString& StringB)
{
	FString Chars;
	Chars.Reserve(StringA.Len() + StringB.Len());

	for (int32 i = 0; i < StringA.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
		if (!StringB.Contains(SingleChar, ESearchCase::CaseSensitive))
		{
			Chars.Append(SingleChar);
		}
	}

	Chars.Shrink();

	return Chars;
}

bool UGdhStringLibrary::IsSubSet(const FString& StringA, const FString& StringB)
{
	for (int32 i = 0; i < StringB.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
		if (!StringB.Contains(SingleChar, ESearchCase::CaseSensitive))
		{
			return false;
		}
	}

	return true;
}

bool UGdhStringLibrary::Contains(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase)
{
	const FString StringA = SearchCase == ESearchCase::CaseSensitive ? OriginalString : OriginalString.ToLower();
	const FString StringB = SearchCase == ESearchCase::CaseSensitive ? Dictionary : Dictionary.ToLower();

	return Intersection(StringA, StringB).Len() > 0;
}

bool UGdhStringLibrary::ContainsOnly(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase)
{
	const FString StringA = SearchCase == ESearchCase::CaseSensitive ? OriginalString : OriginalString.ToLower();
	const FString StringB = SearchCase == ESearchCase::CaseSensitive ? Dictionary : Dictionary.ToLower();

	return IsSubSet(StringA, StringB);
}

bool UGdhStringLibrary::ContainsLetters(const FString& OriginalString)
{
	return Contains(OriginalString, GdhConstants::AlphaLower, ESearchCase::IgnoreCase);
}

bool UGdhStringLibrary::ContainsDigits(const FString& OriginalString)
{
	return Contains(OriginalString, GdhConstants::Digits, ESearchCase::IgnoreCase);
}

bool UGdhStringLibrary::ContainsOnlyLetters(const FString& OriginalString)
{
	return ContainsOnly(OriginalString, GdhConstants::AlphaLower, ESearchCase::IgnoreCase);
}

bool UGdhStringLibrary::ContainsOnlyDigits(const FString& OriginalString)
{
	return ContainsOnly(OriginalString, GdhConstants::Digits, ESearchCase::IgnoreCase);
}

FString UGdhStringLibrary::ConvertNamingCase(const FString& OriginalString, const EGdhNamingCase NamingCase)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	if (NamingCase == EGdhNamingCase::CamelCase)
	{
		return ConvertToCamelCase(OriginalString);
	}

	if (NamingCase == EGdhNamingCase::KebabCase)
	{
		return ConvertToKebabCase(OriginalString);
	}

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

FString UGdhStringLibrary::ConvertToPascalCase(const FString& OriginalString)
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

FString UGdhStringLibrary::ConvertToPascalSnakeCase(const FString& OriginalString)
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

FString UGdhStringLibrary::ConvertToSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	return Tokenize(OriginalString);
}

FString UGdhStringLibrary::ConvertToKebabCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	return UKismetStringLibrary::JoinStringArray(Parts, TEXT("-"));
}

FString UGdhStringLibrary::ConvertToCamelCase(const FString& OriginalString)
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

FString UGdhStringLibrary::Normalize(const FString& OriginalString)
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

FString UGdhStringLibrary::Tokenize(const FString& OriginalString)
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

		// todo:ashe23 this part must be refactored
		if (
			!FChar::IsUnderscore(CurrentChar) && FChar::IsUnderscore(PreviousChar) ||
			FChar::IsLower(CurrentChar) && FChar::IsDigit(PreviousChar) ||
			FChar::IsUpper(CurrentChar) && FChar::IsDigit(PreviousChar) ||
			// FChar::IsUpper(CurrentChar) && FChar::IsLower(PreviousChar) ||
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
