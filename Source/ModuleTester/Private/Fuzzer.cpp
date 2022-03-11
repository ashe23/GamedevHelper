// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Fuzzer.h"

#include "UtilityMathLibrary.h"

FFuzzer::FFuzzer(const int32 Seed)
{
	this->Seed = Seed;
	RandomStream = FRandomStream{this->Seed};
}

FString FFuzzer::GetRandomString(const int32 Len, const FString& Dictionary) const
{
	if (Len <= 0 || Dictionary.IsEmpty())
	{
		return FString{};
	}

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

FString FFuzzer::GetRandomString(const int32 Len, const Fuzzer::EStringFormat StringFormat) const
{
	using namespace Fuzzer;
	if (Len <= 0 || StringFormat == EStringFormat::None)
	{
		return FString{};
	}

	if (StringFormat == EStringFormat::Lower)
	{
		return GetRandomString(Len, Alpha);
	}

	if (StringFormat == EStringFormat::Upper)
	{
		return GetRandomString(Len, Alpha.ToUpper());
	}

	if (StringFormat == EStringFormat::Mixed)
	{
		return GetRandomString(Len, Alpha + Alpha.ToUpper());
	}

	if (StringFormat == EStringFormat::Digit)
	{
		return GetRandomString(Len, Digits);
	}

	if (StringFormat == EStringFormat::Special)
	{
		return GetRandomString(Len, Specials);
	}

	if (StringFormat == EStringFormat::StartUpper)
	{
		const FString FirstLetter = GetRandomString(1, Alpha.ToUpper());
		const FString RestOfStr = GetRandomString(Len - 1, Alpha);
		return FirstLetter + RestOfStr;
	}

	if (StringFormat == EStringFormat::EndUpper)
	{
		const FString LastLetter = GetRandomString(1, Alpha.ToUpper());
		const FString RestOfStr = GetRandomString(Len - 1, Alpha);
		return RestOfStr + LastLetter;
	}

	if (StringFormat == EStringFormat::StartLower)
	{
		const FString FirstLetter = GetRandomString(1, Alpha);
		const FString RestOfStr = GetRandomString(Len - 1, Alpha.ToUpper());
		return FirstLetter + RestOfStr;
	}

	if (StringFormat == EStringFormat::EndLower)
	{
		const FString LastLetter = GetRandomString(1, Alpha);
		const FString RestOfStr = GetRandomString(Len - 1, Alpha.ToUpper());
		return RestOfStr + LastLetter;
	}

	if (StringFormat == EStringFormat::PascalCase)
	{
		TArray<int32> Parts;
		UGamedevHelperMathLibrary::GetIntegerPartitions(Len, RandomStream.RandRange(2,8), Parts);
		
		FString PascalCaseString;
		for (const auto& Part : Parts)
		{
			PascalCaseString += GetRandomString(Part, EStringFormat::StartUpper);
		}
		return PascalCaseString;
	}

	if (StringFormat == EStringFormat::PascalSnakeCase)
	{
	}

	return FString{};
}
