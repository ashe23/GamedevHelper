// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperCoreStringFuzzerLibrary.h"

FString UGamedevHelperStringFuzzerLibrary::GetRandomString(const int32 Len, const FString& Dictionary, const int32 Seed)
{
	if (Len <= 0 || Dictionary.IsEmpty())
	{
		return FString{};
	}

	const FRandomStream RandomStream{Seed};
	const auto Chars = Dictionary.GetCharArray();

	FString FinalString;
	FinalString.Reserve(Len);

	for (int32 i = 0; i < Len; ++i)
	{
		// chars - ["a", "b", "c", "\0"]
		// num - 4
		// rand range must be [0:num-2], ignoring '\0' char
		const int32 RandIndex = RandomStream.RandRange(0, Chars.Num() - 2);
		FinalString.AppendChar(Chars[RandIndex]);
	}

	return FinalString;
}

FString UGamedevHelperStringFuzzerLibrary::GetRandomString(const int32 Len, const EGDHFuzzerStringFormat StringFormat, const int32 Seed)
{
	if (Len <= 0 || StringFormat == EGDHFuzzerStringFormat::None)
	{
		return FString{};
	}
	
	static const FString Alpha = TEXT("abcdefghijklmnopqrstuvwxyz");
	static const FString Digits = TEXT("0123456789");
	static const FString Specials = TEXT("_-");

	if (StringFormat == EGDHFuzzerStringFormat::Lower)
	{
		return GetRandomString(Len, Alpha, Seed);
	}

	if (StringFormat == EGDHFuzzerStringFormat::Upper)
	{
		return GetRandomString(Len, Alpha.ToUpper(), Seed);
	}

	if (StringFormat == EGDHFuzzerStringFormat::Mixed)
	{
		return GetRandomString(Len, Alpha + Alpha.ToUpper(), Seed);
	}

	if (StringFormat == EGDHFuzzerStringFormat::Digit)
	{
		return GetRandomString(Len, Digits, Seed);
	}

	if (StringFormat == EGDHFuzzerStringFormat::Special)
	{
		return GetRandomString(Len, Specials, Seed);
	}

	if (StringFormat == EGDHFuzzerStringFormat::StartUpper)
	{
		const FString FirstLetter = GetRandomString(1, Alpha.ToUpper(), Seed);
		const FString RestOfStr = GetRandomString(Len - 1, Alpha, Seed);
		return FirstLetter + RestOfStr;
	}

	if (StringFormat == EGDHFuzzerStringFormat::EndUpper)
	{
		const FString LastLetter = GetRandomString(1, Alpha.ToUpper(), Seed);
		const FString RestOfStr = GetRandomString(Len - 1, Alpha, Seed);
		return RestOfStr + LastLetter;
	}

	if (StringFormat == EGDHFuzzerStringFormat::StartLower)
	{
		const FString FirstLetter = GetRandomString(1, Alpha, Seed);
		const FString RestOfStr = GetRandomString(Len - 1, Alpha.ToUpper(), Seed);
		return FirstLetter + RestOfStr;
	}

	if (StringFormat == EGDHFuzzerStringFormat::EndLower)
	{
		const FString LastLetter = GetRandomString(1, Alpha, Seed);
		const FString RestOfStr = GetRandomString(Len - 1, Alpha.ToUpper(), Seed);
		return RestOfStr + LastLetter;
	}

	// todo:ashe23 implement for other formats
	// if (StringFormat == EGDHFuzzerStringFormat::PascalCase)
	// {
	// 	TArray<int32> Parts;
	// 	UGamedevHelperMathLibrary::GetIntegerPartitions(Len, RandomStream.RandRange(2,8), Parts);
	// 	
	// 	FString PascalCaseString;
	// 	for (const auto& Part : Parts)
	// 	{
	// 		PascalCaseString += GetRandomString(Part, EGDHFuzzerStringFormat::StartUpper);
	// 	}
	// 	return PascalCaseString;
	// }

	return FString{};
}
