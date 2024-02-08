#pragma once

#include "CoreTypes.h"
#include "GdhConstants.h"
#include "Misc/AutomationTest.h"
#include "GdhLibString.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringRepeat, "Gdh.Library.String.Repeat", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringRandom, "Gdh.Library.String.Random", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasAny, "Gdh.Library.String.HasAny", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)


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

bool FGdhLibStringHasAny::RunTest(const FString& Parameters)
{
	// case insensitive
	TestFalse(TEXT("[case insensitive] Empty Str And Empty Charset"), UGdhLibString::HasAny(TEXT(""), TEXT(""), ESearchCase::IgnoreCase));
	TestFalse(TEXT("[case insensitive] Empty Str And Non Empty Charset"), UGdhLibString::HasAny(TEXT(""), TEXT("abc"), ESearchCase::IgnoreCase));
	TestFalse(TEXT("[case insensitive] Non Empty Str And Empty Charset"), UGdhLibString::HasAny(TEXT("abc"), TEXT(""), ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Ascii chars #1"), UGdhLibString::HasAny(TEXT("abc"), TEXT("a"), ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Ascii chars #2"), UGdhLibString::HasAny(TEXT("abc"), TEXT("b"), ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Ascii chars #3"), UGdhLibString::HasAny(TEXT("abc"), TEXT("ab"), ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Ascii chars #4"), UGdhLibString::HasAny(TEXT("abc"), TEXT("abc"), ESearchCase::IgnoreCase));
	TestFalse(TEXT("[case insensitive] Ascii chars #5"), UGdhLibString::HasAny(TEXT("abc"), TEXT("e"), ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Special chars #1"), UGdhLibString::HasAny(TEXT("[]"), GdhConstants::SpecialChars, ESearchCase::IgnoreCase));
	TestFalse(TEXT("[case insensitive] Special chars #2"), UGdhLibString::HasAny(TEXT("[]"), GdhConstants::AlphaMixed, ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Unicode chars #1"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("ПР"), ESearchCase::IgnoreCase));
	TestFalse(TEXT("[case insensitive] Unicode chars #2"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("ООО"), ESearchCase::IgnoreCase));
	TestTrue(TEXT("[case insensitive] Unicode chars #3"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("РИВЕТ"), ESearchCase::IgnoreCase));

	// case sensitive
	TestFalse(TEXT("[case sensitive] Empty Str And Empty Charset"), UGdhLibString::HasAny(TEXT(""), TEXT(""), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Empty Str And Non Empty Charset"), UGdhLibString::HasAny(TEXT(""), TEXT("abc"), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Non Empty Str And Empty Charset"), UGdhLibString::HasAny(TEXT("abc"), TEXT(""), ESearchCase::CaseSensitive));
	TestTrue(TEXT("[case sensitive] Ascii chars #1"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("A"), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Ascii chars #2"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("b"), ESearchCase::CaseSensitive));
	TestTrue(TEXT("[case sensitive] Ascii chars #3"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("Ab"), ESearchCase::CaseSensitive));
	TestTrue(TEXT("[case sensitive] Ascii chars #4"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("ABC"), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Ascii chars #5"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("E"), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Ascii chars #6"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("e"), ESearchCase::CaseSensitive));
	TestTrue(TEXT("[case sensitive] Special chars #1"), UGdhLibString::HasAny(TEXT("[]"), GdhConstants::SpecialChars, ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Special chars #2"), UGdhLibString::HasAny(TEXT("[]"), GdhConstants::AlphaMixed, ESearchCase::CaseSensitive));
	TestTrue(TEXT("[case sensitive] Unicode chars #1"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("ПР"), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Unicode chars #2"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("ООО"), ESearchCase::CaseSensitive));
	TestFalse(TEXT("[case sensitive] Unicode chars #3"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("РИВЕТ"), ESearchCase::CaseSensitive));
	
	return true;
}


#endif
