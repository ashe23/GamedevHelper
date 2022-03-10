// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Fuzzer.h"

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
