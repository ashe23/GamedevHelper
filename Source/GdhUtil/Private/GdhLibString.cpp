// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibString.h"
// Engine Headers
#include "GdhConstants.h"
#include "GdhEnums.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/AsciiSet.h"

// STRING CREATION

FString UGdhLibString::GetCharsetAlphaLower()
{
	return GdhConstants::AlphaLower;
}

FString UGdhLibString::GetCharsetAlphaUpper()
{
	return GdhConstants::AlphaUpper;
}

FString UGdhLibString::GetCharsetAlphaLowerAndUpper()
{
	return GdhConstants::AlphaLower + GdhConstants::AlphaUpper;
}

FString UGdhLibString::GetCharsetDigits()
{
	return GdhConstants::Digits;
}

FString UGdhLibString::GetCharsetMixed()
{
	return GdhConstants::AlphaMixed + GdhConstants::Digits;
}

FString UGdhLibString::GetCharsetSpecial()
{
	return GdhConstants::SpecialChars;
}

FString UGdhLibString::Repeat(const FString& Str, const int32 Num)
{
	if (Str.IsEmpty() || Num == 0) return Str;

	FString Result;
	Result.Reserve(Str.Len() * Num);

	for (int32 i = 0; i < Num; ++i)
	{
		Result.Append(Str);
	}

	return Result;
}

FString UGdhLibString::Random(const int32 Len, const FString& Charset, const int32 Seed)
{
	if (Len == 0 || Charset.IsEmpty()) return {};

	FRandomStream RandomStream{Seed};
	if (Seed == 0)
	{
		RandomStream.GenerateNewSeed();
	}
	const auto Chars = Charset.GetCharArray();

	FString FinalString;
	FinalString.Reserve(Len);

	for (int32 Index = 0; Index < Len; ++Index)
	{
		const int32 RandIndex = RandomStream.RandRange(0, Chars.Num() - 2); // ignore '\0' char
		FinalString.AppendChar(Chars[RandIndex]);
	}

	return FinalString;
}

bool UGdhLibString::HasAny(const FString& Str, const FString& Charset)
{
	int32 Index = INDEX_NONE;

	for (const auto& Char : Str)
	{
		if (Charset.FindChar(Char, Index))
		{
			return true;
		}
	}

	return false;
}

bool UGdhLibString::HasNone(const FString& Str, const FString& Charset)
{
	return !HasAny(Str, Charset);
}

bool UGdhLibString::HasOnly(const FString& Str, const FString& Charset)
{
	if (Str.IsEmpty() || Charset.IsEmpty()) return false;

	int32 Index = INDEX_NONE;
	for (const auto& Char : Str)
	{
		if (!Charset.FindChar(Char, Index))
		{
			return false;
		}
	}

	return true;
}

bool UGdhLibString::HasAscii(const FString& Str)
{
	if (Str.IsEmpty()) return false;

	constexpr FAsciiSet WhitespaceCharacters(" \v\f\t\r\n");
	
	for (const auto& Char : Str)
	{
		if (WhitespaceCharacters.Contains(Char))
		{
			return true;
		}
		
		const uint32 CharValue = TChar<TCHAR>::ToUnsigned(Char);
		if (CharValue > 0 && CharValue <= 127)
		{
			return true;
		}
	}

	return false;
}

bool UGdhLibString::HasOnlyAscii(const FString& Str)
{
	if (Str.IsEmpty()) return false;
	
	for (const auto& Char : Str)
	{
		const uint32 CharValue = TChar<TCHAR>::ToUnsigned(Char);
		if (CharValue > 127)
		{
			return false;
		}
	}

	return true;
}

bool UGdhLibString::HasUnicode(const FString& Str)
{
	if (Str.IsEmpty()) return false;
	
	return !HasOnlyAscii(Str);
}

bool UGdhLibString::HasOnlyUnicode(const FString& Str)
{
	if (Str.IsEmpty()) return false;
	
	return !HasAscii(Str);
}

FString UGdhLibString::Intersection(const FString& StringA, const FString& StringB)
{
	if (StringA.IsEmpty() || StringB.IsEmpty()) return {};

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

FString UGdhLibString::Union(const FString& StringA, const FString& StringB)
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

FString UGdhLibString::SymmetricDifference(const FString& StringA, const FString& StringB)
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

FString UGdhLibString::Difference(const FString& StringA, const FString& StringB)
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

bool UGdhLibString::IsSubSet(const FString& StringA, const FString& StringB)
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

FString UGdhLibString::Normalize(const FString& OriginalString)
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

FString UGdhLibString::Tokenize(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Normalized = Normalize(OriginalString);
	if (Normalized.IsEmpty()) return {};

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

FString UGdhLibString::ConvertNamingCase(const FString& OriginalString, const EGdhNamingCase NamingCase)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	switch (NamingCase)
	{
		case EGdhNamingCase::None: return OriginalString;
		case EGdhNamingCase::PascalSnakeCase: return ConvertToPascalSnakeCase(OriginalString);
		case EGdhNamingCase::PascalCase: return ConvertToPascalCase(OriginalString);
		case EGdhNamingCase::UpperCase: return ConvertToUpperCase(OriginalString);
		case EGdhNamingCase::LowerCase: return ConvertToLowerCase(OriginalString);
		case EGdhNamingCase::SnakeCase: return ConvertToSnakeCase(OriginalString);
		case EGdhNamingCase::CamelCase: return ConvertToCamelCase(OriginalString);
		case EGdhNamingCase::KebabCase: return ConvertToKebabCase(OriginalString);
		default: return OriginalString;
	}
}

FString UGdhLibString::ConvertToPascalCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	if (Tokenized.IsEmpty()) return {};

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

FString UGdhLibString::ConvertToUpperCase(const FString& OriginalString)
{
	return Tokenize(OriginalString).ToUpper();
}

FString UGdhLibString::ConvertToLowerCase(const FString& OriginalString)
{
	return Tokenize(OriginalString).ToLower();
}

FString UGdhLibString::ConvertToSnakeCase(const FString& OriginalString)
{
	return Tokenize(OriginalString);
}

FString UGdhLibString::ConvertToCamelCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	if (Tokenized.IsEmpty()) return {};

	TArray<FString> Parts;
	Tokenized.ParseIntoArray(Parts, TEXT("_"));

	TArray<FString> CapitalizedParts;
	CapitalizedParts.Reserve(Parts.Num());

	int32 Index = 0;
	for (const auto& Part : Parts)
	{
		const FString FirstLetter = Index == 0 ? UKismetStringLibrary::GetSubstring(Part, 0, 1).ToLower() : UKismetStringLibrary::GetSubstring(Part, 0, 1).ToUpper();
		const FString RestOfStr = UKismetStringLibrary::GetSubstring(Part, 1, Part.Len() - 1).ToLower();
		CapitalizedParts.Add(FirstLetter + RestOfStr);

		++Index;
	}

	return UKismetStringLibrary::JoinStringArray(CapitalizedParts, TEXT(""));
}

FString UGdhLibString::ConvertToKebabCase(const FString& OriginalString)
{
	const FString SnakeCase = ConvertToSnakeCase(OriginalString);
	return SnakeCase.Replace(TEXT("_"), TEXT("-"));
}

FString UGdhLibString::ConvertToPascalSnakeCase(const FString& OriginalString)
{
	if (OriginalString.IsEmpty()) return OriginalString;

	const FString Tokenized = Tokenize(OriginalString);
	if (Tokenized.IsEmpty()) return {};

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
