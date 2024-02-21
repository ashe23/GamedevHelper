// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibString.h"
#include "GdhConstants.h"
#include "GdhEnums.h"
// Engine Headers
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

	const TSet<TCHAR> StrASet{StringA.GetCharArray()};
	const TSet<TCHAR> StrBSet{StringB.GetCharArray()};
	const TSet<TCHAR> Intersection = StrASet.Intersect(StrBSet);

	FString Final;
	Final.Reserve(Intersection.Num());

	// Iterate through StringA to maintain the order of characters
	for (const auto& Char : StringA)
	{
		if (Intersection.Contains(Char))
		{
			Final.Append(FString::Chr(Char));
		}
	}

	Final.Shrink();

	return Final;
}

FString UGdhLibString::Union(const FString& StringA, const FString& StringB)
{
	if (StringA.IsEmpty() && StringB.IsEmpty()) return {};

	const TSet<TCHAR> StrASet{StringA.GetCharArray()};
	const TSet<TCHAR> StrBSet{StringB.GetCharArray()};
	const TSet<TCHAR> Union = StrASet.Union(StrBSet);

	FString Final;
	Final.Reserve(Union.Num());

	for (const auto& Char : Union)
	{
		Final.Append(FString::Chr(Char));
	}

	Final.Shrink();

	return Final;
}

FString UGdhLibString::Difference(const FString& StringA, const FString& StringB)
{
	FString Chars;
	Chars.Reserve(StringA.Len() + StringB.Len());

	int32 Index = INDEX_NONE;

	for (const auto& Char : StringA)
	{
		if (Chars.FindChar(Char, Index)) continue;

		if (!StringB.FindChar(Char, Index))
		{
			Chars.Append(FString::Chr(Char));
		}
	}

	Chars.Shrink();

	return Chars;
}

FString UGdhLibString::SymmetricDifference(const FString& StringA, const FString& StringB)
{
	// Symmetric difference = union - intersection
	const FString Union = UGdhLibString::Union(StringA, StringB);
	const FString Intersection = UGdhLibString::Intersection(StringA, StringB);
	return Difference(Union, Intersection);
}

FString UGdhLibString::Normalize(const FString& Str)
{
	if (Str.IsEmpty()) return Str;

	FString CleanedString;
	CleanedString.Reserve(Str.Len());

	const auto Chars = Str.GetCharArray();
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

FString UGdhLibString::Tokenize(const FString& Str)
{
	if (Str.IsEmpty()) return Str;

	const FString Normalized = Normalize(Str);
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

		if (!FChar::IsUnderscore(CurrentChar) && FChar::IsUnderscore(PreviousChar) || FChar::IsUpper(CurrentChar) && FChar::IsLower(PreviousChar))
		{
			Tokens.Add(Token);
			Token.Empty();
		}

		Token.AppendChar(CurrentChar);
	}

	Tokens.Add(Token);

	return UKismetStringLibrary::JoinStringArray(Tokens, TEXT("_")).ToLower();
}

FString UGdhLibString::ConvertNamingCase(const FString& Str, const EGdhNamingCase NamingCase)
{
	if (Str.IsEmpty()) return Str;

	switch (NamingCase)
	{
		case EGdhNamingCase::None: return Str;
		case EGdhNamingCase::PascalSnakeCase: return ConvertToPascalSnakeCase(Str);
		case EGdhNamingCase::PascalCase: return ConvertToPascalCase(Str);
		case EGdhNamingCase::UpperCase: return ConvertToUpperCase(Str);
		case EGdhNamingCase::LowerCase: return ConvertToLowerCase(Str);
		case EGdhNamingCase::SnakeCase: return ConvertToSnakeCase(Str);
		case EGdhNamingCase::CamelCase: return ConvertToCamelCase(Str);
		case EGdhNamingCase::KebabCase: return ConvertToKebabCase(Str);
		default: return Str;
	}
}

FString UGdhLibString::ConvertToPascalCase(const FString& Str)
{
	if (Str.IsEmpty()) return Str;

	const FString Tokenized = Tokenize(Str);
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

FString UGdhLibString::ConvertToUpperCase(const FString& Str)
{
	return Tokenize(Str).ToUpper();
}

FString UGdhLibString::ConvertToLowerCase(const FString& Str)
{
	return Tokenize(Str).ToLower();
}

FString UGdhLibString::ConvertToSnakeCase(const FString& Str)
{
	return Tokenize(Str);
}

FString UGdhLibString::ConvertToCamelCase(const FString& Str)
{
	if (Str.IsEmpty()) return Str;

	const FString Tokenized = Tokenize(Str);
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

FString UGdhLibString::ConvertToKebabCase(const FString& Str)
{
	const FString SnakeCase = ConvertToSnakeCase(Str);
	return SnakeCase.Replace(TEXT("_"), TEXT("-"));
}

FString UGdhLibString::ConvertToPascalSnakeCase(const FString& Str)
{
	if (Str.IsEmpty()) return Str;

	const FString Tokenized = Tokenize(Str);
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
