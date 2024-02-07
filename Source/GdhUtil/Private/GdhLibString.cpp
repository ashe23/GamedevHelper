// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibString.h"

FString UGdhLibString::GetRandomStringFromCharset(const int32 Len, const FString& Charset, const int32 Seed)
{
	if (Len == 0 || Charset.IsEmpty()) return {};

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
