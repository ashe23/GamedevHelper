// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperStringLibrary.h"
// Engine Headers
#include "GamedevHelper.h"
#include "Kismet/KismetStringLibrary.h"
#include "Internationalization/Regex.h"

FString UGamedevHelperStringLibrary::GetRandomStringFromCharset(const int32 Len, const FString& Charset, const int32 Seed)
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

FString UGamedevHelperStringLibrary::GetFirstLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, 0, 1);
}

FString UGamedevHelperStringLibrary::GetLastLetter(const FString& OriginalString)
{
	if (OriginalString.IsEmpty())
	{
		return OriginalString;
	}

	return UKismetStringLibrary::GetSubstring(OriginalString, OriginalString.Len() - 1, 1);
}

FString UGamedevHelperStringLibrary::RemoveFirstLetter(const FString& OriginalString)
{
	return OriginalString.RightChop(1);
}

FString UGamedevHelperStringLibrary::RemoveLastLetter(const FString& OriginalString)
{
	return OriginalString.LeftChop(1);
}

FString UGamedevHelperStringLibrary::Intersection(const FString& StringA, const FString& StringB)
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

FString UGamedevHelperStringLibrary::Union(const FString& StringA, const FString& StringB)
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

FString UGamedevHelperStringLibrary::SymmetricDifference(const FString& StringA, const FString& StringB)
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

FString UGamedevHelperStringLibrary::Difference(const FString& StringA, const FString& StringB)
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

bool UGamedevHelperStringLibrary::IsSubSet(const FString& StringA, const FString& StringB)
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

bool UGamedevHelperStringLibrary::Contains(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase)
{
	const FString StringA = SearchCase == ESearchCase::CaseSensitive ? OriginalString : OriginalString.ToLower();
	const FString StringB = SearchCase == ESearchCase::CaseSensitive ? Dictionary : Dictionary.ToLower();

	return Intersection(StringA, StringB).Len() > 0;
}

bool UGamedevHelperStringLibrary::ContainsOnly(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase)
{
	const FString StringA = SearchCase == ESearchCase::CaseSensitive ? OriginalString : OriginalString.ToLower();
	const FString StringB = SearchCase == ESearchCase::CaseSensitive ? Dictionary : Dictionary.ToLower();

	return IsSubSet(StringA, StringB);
}

bool UGamedevHelperStringLibrary::ContainsLetters(const FString& OriginalString)
{
	return Contains(OriginalString, GamedevHelperConstants::AlphaLower, ESearchCase::IgnoreCase);
}

bool UGamedevHelperStringLibrary::ContainsDigits(const FString& OriginalString)
{
	return Contains(OriginalString, GamedevHelperConstants::Digits, ESearchCase::IgnoreCase);
}

bool UGamedevHelperStringLibrary::ContainsOnlyLetters(const FString& OriginalString)
{
	return ContainsOnly(OriginalString, GamedevHelperConstants::AlphaLower, ESearchCase::IgnoreCase);
}

bool UGamedevHelperStringLibrary::ContainsOnlyDigits(const FString& OriginalString)
{
	return ContainsOnly(OriginalString, GamedevHelperConstants::Digits, ESearchCase::IgnoreCase);
}

FString UGamedevHelperStringLibrary::RegexReplace(const FString& OriginalString, const FString& Pattern, const FString& Substitution)
{
	if (OriginalString.IsEmpty() || Pattern.IsEmpty()) return FString{};

	const FRegexPattern RegexPattern{Pattern};
	FRegexMatcher RegexMatcher{RegexPattern, OriginalString};

	FString FinalString = OriginalString;
	
	while(RegexMatcher.FindNext())
	{
		const FString Token = RegexMatcher.GetCaptureGroup(0);
		FinalString = FinalString.Replace(*Token, *Substitution);
	}
	
	return FinalString;
}
