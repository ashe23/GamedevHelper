// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "UtilityStringLibrary.h"

#include "Kismet/KismetStringLibrary.h"

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
