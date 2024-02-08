#pragma once

#include "CoreTypes.h"
#include "GdhConstants.h"
#include "Misc/AutomationTest.h"
#include "GdhLibString.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringRepeat, "Gdh.Library.String.Repeat", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringRandom, "Gdh.Library.String.Random", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)


bool FGdhLibStringRepeat::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty input and num is zero"), UGdhLibString::Repeat(TEXT(""), 0), TEXT(""));
	TestEqual(TEXT("Empty input and num > 0"), UGdhLibString::Repeat(TEXT(""), 23), TEXT(""));
	TestEqual(TEXT("Input and num is zero"), UGdhLibString::Repeat(TEXT("aaa"), 0), TEXT("aaa"));
	TestEqual(TEXT("Repetition text check"), UGdhLibString::Repeat(TEXT("aaa"), 2), TEXT("aaaaaa"));
	TestTrue(TEXT("Repetition length check"), UGdhLibString::Repeat(TEXT("a"), 10).Len() == 10);
	TestEqual(TEXT("Different naming case"), UGdhLibString::Repeat(TEXT("AbC"), 2), TEXT("AbCAbC"));
	TestEqual(TEXT("String with different encodings"), UGdhLibString::Repeat(TEXT("AИԲ"), 2), TEXT("AИԲAИԲ"));
	TestTrue(TEXT("Large strings"), UGdhLibString::Repeat(TEXT("a"), 10000).Len() == 10000);
	TestEqual(TEXT("Special characters"), UGdhLibString::Repeat(TEXT("-`~!@#$%^&*()+=[]{}\\|/.,;:'\"?"), 1), TEXT("-`~!@#$%^&*()+=[]{}\\|/.,;:'\"?"));
	TestEqual(TEXT("Strings with newline characters"), UGdhLibString::Repeat(TEXT("abc\n"), 2), TEXT("abc\nabc\n"));
	TestEqual(TEXT("Strings with whitespace characters"), UGdhLibString::Repeat(TEXT("abc def "), 2), TEXT("abc def abc def "));

	return true;
}

bool FGdhLibStringRandom::RunTest(const FString& Parameters)
{
	// NO SEED
	TestEqual(TEXT("Empty Charset and Len = 0"), UGdhLibString::Random(0, TEXT("")), TEXT(""));
	TestEqual(TEXT("Empty Charset and Len > 0"), UGdhLibString::Random(10, TEXT("")), TEXT(""));
	TestEqual(TEXT("Charset and Len = 0"), UGdhLibString::Random(0, TEXT("abc")), TEXT(""));

	for (int32 i = 1; i < 1000; ++i)
	{
		TestTrue(TEXT("Ascii charset"), UGdhLibString::Random(i, TEXT("abc")).Len() == i);
		TestTrue(TEXT("Ascii charset different cases"), UGdhLibString::Random(i, TEXT("abcABC")).Len() == i);
		TestTrue(TEXT("Unicode charset #1"), UGdhLibString::Random(i, TEXT("Привет")).Len() == i);
		TestTrue(TEXT("Unicode charset #2"), UGdhLibString::Random(i, TEXT("Բարև")).Len() == i);
		TestTrue(TEXT("Mixed charset"), UGdhLibString::Random(i, TEXT("HelloԲարև")).Len() == i);
		TestTrue(TEXT("Special chars"), UGdhLibString::Random(i, TEXT("-`~!@#$%^&*()+=[]{}\\|/.,;:'\"?")).Len() == i);
		TestTrue(TEXT("NewLine chars"), UGdhLibString::Random(i, TEXT("abc\n")).Len() == i);
		TestTrue(TEXT("Whitespace chars"), UGdhLibString::Random(i, TEXT("abc ")).Len() == i);
	}

	// WITH SEED
	constexpr int32 Seed = 23;
	TestEqual(TEXT("Seed version"), UGdhLibString::Random(10, GdhConstants::AlphaMixed, Seed), UGdhLibString::Random(10, GdhConstants::AlphaMixed, Seed));

	return true;
}


#endif
