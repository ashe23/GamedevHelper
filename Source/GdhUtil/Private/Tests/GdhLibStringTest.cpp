#pragma once

#include "CoreTypes.h"
#include "GdhConstants.h"
#include "Misc/AutomationTest.h"
#include "GdhLibString.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringRepeat, "Gdh.Library.String.Repeat", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringRandom, "Gdh.Library.String.Random", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasAny, "Gdh.Library.String.HasAny", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasNone, "Gdh.Library.String.HasNone", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasOnly, "Gdh.Library.String.HasOnly", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasAscii, "Gdh.Library.String.HasAscii", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasOnlyAscii, "Gdh.Library.String.HasOnlyAscii", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasUnicode, "Gdh.Library.String.HasUnicode", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringHasOnlyUnicode, "Gdh.Library.String.HasOnlyUnicode", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)


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
	TestFalse(TEXT("Empty Str And Empty Charset"), UGdhLibString::HasAny(TEXT(""), TEXT("")));
	TestFalse(TEXT("Empty Str And Non Empty Charset"), UGdhLibString::HasAny(TEXT(""), TEXT("abc")));
	TestFalse(TEXT("Non Empty Str And Empty Charset"), UGdhLibString::HasAny(TEXT("abc"), TEXT("")));
	TestTrue(TEXT("Ascii chars #1"), UGdhLibString::HasAny(TEXT("abc"), TEXT("a")));
	TestTrue(TEXT("Ascii chars #2"), UGdhLibString::HasAny(TEXT("abc"), TEXT("b")));
	TestTrue(TEXT("Ascii chars #3"), UGdhLibString::HasAny(TEXT("abc"), TEXT("ab")));
	TestTrue(TEXT("Ascii chars #4"), UGdhLibString::HasAny(TEXT("abc"), TEXT("abc")));
	TestFalse(TEXT("Ascii chars #5"), UGdhLibString::HasAny(TEXT("abc"), TEXT("e")));
	TestFalse(TEXT("Ascii chars #6"), UGdhLibString::HasAny(TEXT("ABC"), TEXT("abc")));
	TestTrue(TEXT("Ascii chars #7"), UGdhLibString::HasAny(TEXT("ABc"), TEXT("abc")));
	TestTrue(TEXT("Special chars #1"), UGdhLibString::HasAny(TEXT("[]"), GdhConstants::SpecialChars));
	TestFalse(TEXT("Special chars #2"), UGdhLibString::HasAny(TEXT("[]"), GdhConstants::AlphaMixed));
	TestTrue(TEXT("Unicode chars #1"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("ПР")));
	TestFalse(TEXT("Unicode chars #2"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("ООО")));
	TestFalse(TEXT("Unicode chars #3"), UGdhLibString::HasAny(TEXT("Привет"), TEXT("РИВЕТ")));

	return true;
}

bool FGdhLibStringHasNone::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Empty Str And Empty Charset"), UGdhLibString::HasNone(TEXT(""), TEXT("")));
	TestTrue(TEXT("Empty Str And Non Empty Charset"), UGdhLibString::HasNone(TEXT(""), TEXT("abc")));
	TestTrue(TEXT("Non Empty Str And Empty Charset"), UGdhLibString::HasNone(TEXT("abc"), TEXT("")));
	TestFalse(TEXT("Ascii chars #1"), UGdhLibString::HasNone(TEXT("abc"), TEXT("a")));
	TestFalse(TEXT("Ascii chars #2"), UGdhLibString::HasNone(TEXT("abc"), TEXT("b")));
	TestFalse(TEXT("Ascii chars #3"), UGdhLibString::HasNone(TEXT("abc"), TEXT("ab")));
	TestFalse(TEXT("Ascii chars #4"), UGdhLibString::HasNone(TEXT("abc"), TEXT("abc")));
	TestTrue(TEXT("Ascii chars #5"), UGdhLibString::HasNone(TEXT("abc"), TEXT("e")));
	TestTrue(TEXT("Ascii chars #6"), UGdhLibString::HasNone(TEXT("ABC"), TEXT("abc")));
	TestFalse(TEXT("Ascii chars #7"), UGdhLibString::HasNone(TEXT("ABc"), TEXT("abc")));
	TestFalse(TEXT("Special chars #1"), UGdhLibString::HasNone(TEXT("[]"), GdhConstants::SpecialChars));
	TestTrue(TEXT("Special chars #2"), UGdhLibString::HasNone(TEXT("[]"), GdhConstants::AlphaMixed));
	TestFalse(TEXT("Unicode chars #1"), UGdhLibString::HasNone(TEXT("Привет"), TEXT("ПР")));
	TestTrue(TEXT("Unicode chars #2"), UGdhLibString::HasNone(TEXT("Привет"), TEXT("ООО")));
	TestTrue(TEXT("Unicode chars #3"), UGdhLibString::HasNone(TEXT("Привет"), TEXT("РИВЕТ")));


	return true;
}

bool FGdhLibStringHasOnly::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Empty Str And Empty Charset"), UGdhLibString::HasOnly(TEXT(""), TEXT("")));
	TestFalse(TEXT("Empty Str And Non Empty Charset"), UGdhLibString::HasOnly(TEXT(""), TEXT("abc")));
	TestFalse(TEXT("Non Empty Str And Empty Charset"), UGdhLibString::HasOnly(TEXT(""), TEXT("")));
	TestTrue(TEXT("Ascii chars #1"), UGdhLibString::HasOnly(TEXT("a"), TEXT("abc")));
	TestTrue(TEXT("Ascii chars #2"), UGdhLibString::HasOnly(TEXT("ab"), TEXT("abc")));
	TestTrue(TEXT("Ascii chars #3"), UGdhLibString::HasOnly(TEXT("abc"), TEXT("abc")));
	TestFalse(TEXT("Ascii chars #4"), UGdhLibString::HasOnly(TEXT("abcd"), TEXT("abc")));
	TestTrue(TEXT("Ascii chars #5"), UGdhLibString::HasOnly(TEXT("aaa"), TEXT("a")));
	TestFalse(TEXT("Ascii chars #6"), UGdhLibString::HasOnly(TEXT("ABc"), TEXT("abc")));
	TestTrue(TEXT("Special chars #1"), UGdhLibString::HasOnly(TEXT("[]"), GdhConstants::SpecialChars));
	TestFalse(TEXT("Special chars #2"), UGdhLibString::HasOnly(TEXT("[]a"), GdhConstants::SpecialChars));
	TestTrue(TEXT("Unicode chars #1"), UGdhLibString::HasOnly(TEXT("Привет"), TEXT("Привет")));
	TestFalse(TEXT("Unicode chars #1"), UGdhLibString::HasOnly(TEXT("Привет"), TEXT("ПРивеот")));
	TestFalse(TEXT("Unicode chars #2"), UGdhLibString::HasOnly(TEXT("Привет "), TEXT("Привет"))); // Space at the end of Str.

	return true;
}

bool FGdhLibStringHasAscii::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Empty String"), UGdhLibString::HasAscii(TEXT("")));
	TestTrue(TEXT("ASCII Only"), UGdhLibString::HasAscii(TEXT("Hello, world!")));
	TestTrue(TEXT("Mixed With ASCII At Start"), UGdhLibString::HasAscii(TEXT("AПривет")));
	TestTrue(TEXT("Mixed With ASCII In Middle"), UGdhLibString::HasAscii(TEXT("ПривAет")));
	TestTrue(TEXT("Mixed With ASCII At End"), UGdhLibString::HasAscii(TEXT("ПриветA")));
	TestFalse(TEXT("Non-ASCII Only"), UGdhLibString::HasAscii(TEXT("Привет")));
	TestTrue(TEXT("Numbers"), UGdhLibString::HasAscii(TEXT("12345")));
	TestTrue(TEXT("Special Characters"), UGdhLibString::HasAscii(TEXT("!@#$%^")));
	TestFalse(TEXT("Extended ASCII"), UGdhLibString::HasAscii(TEXT("ÿüöäß")));
	TestTrue(TEXT("Newline Character"), UGdhLibString::HasAscii(TEXT("\n")));
	TestTrue(TEXT("Tab Character"), UGdhLibString::HasAscii(TEXT("\t")));
	TestTrue(TEXT("Carriage Return"), UGdhLibString::HasAscii(TEXT("\r")));

	return true;
}

bool FGdhLibStringHasOnlyAscii::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Empty String"), UGdhLibString::HasOnlyAscii(TEXT("")));
	TestTrue(TEXT("ASCII Only"), UGdhLibString::HasOnlyAscii(TEXT("Hello, world!")));
	TestFalse(TEXT("Mixed With ASCII At Start"), UGdhLibString::HasOnlyAscii(TEXT("AПривет")));
	TestFalse(TEXT("Mixed With ASCII In Middle"), UGdhLibString::HasOnlyAscii(TEXT("ПривAет")));
	TestFalse(TEXT("Mixed With ASCII At End"), UGdhLibString::HasOnlyAscii(TEXT("ПриветA")));
	TestFalse(TEXT("Non-ASCII Only"), UGdhLibString::HasOnlyAscii(TEXT("Привет")));
	TestTrue(TEXT("Numbers"), UGdhLibString::HasOnlyAscii(TEXT("12345")));
	TestTrue(TEXT("Special Characters"), UGdhLibString::HasOnlyAscii(TEXT("!@#$%^")));
	TestFalse(TEXT("Extended ASCII"), UGdhLibString::HasOnlyAscii(TEXT("ÿüöäß")));

	return true;
}

bool FGdhLibStringHasUnicode::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Empty String"), UGdhLibString::HasUnicode(TEXT("")));
	TestFalse(TEXT("ASCII Only"), UGdhLibString::HasUnicode(TEXT("Hello, world!")));
	TestTrue(TEXT("Mixed With ASCII At Start"), UGdhLibString::HasUnicode(TEXT("AПривет")));
	TestTrue(TEXT("Mixed With ASCII In Middle"), UGdhLibString::HasUnicode(TEXT("ПривAет")));
	TestTrue(TEXT("Mixed With ASCII At End"), UGdhLibString::HasUnicode(TEXT("ПриветA")));
	TestTrue(TEXT("Non-ASCII Only"), UGdhLibString::HasUnicode(TEXT("Привет")));
	TestFalse(TEXT("Numbers"), UGdhLibString::HasUnicode(TEXT("12345")));
	TestFalse(TEXT("Special Characters"), UGdhLibString::HasUnicode(TEXT("!@#$%^")));
	TestTrue(TEXT("Extended ASCII"), UGdhLibString::HasUnicode(TEXT("ÿüöäß")));

	return true;
}


bool FGdhLibStringHasOnlyUnicode::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Empty String"), UGdhLibString::HasOnlyUnicode(TEXT("")));
	TestFalse(TEXT("ASCII Only"), UGdhLibString::HasOnlyUnicode(TEXT("Hello, world!")));
	TestFalse(TEXT("Mixed With ASCII At Start"), UGdhLibString::HasOnlyUnicode(TEXT("AПривет")));
	TestFalse(TEXT("Mixed With ASCII In Middle"), UGdhLibString::HasOnlyUnicode(TEXT("ПривAет")));
	TestFalse(TEXT("Mixed With ASCII At End"), UGdhLibString::HasOnlyUnicode(TEXT("ПриветA")));
	TestTrue(TEXT("Non-ASCII Only"), UGdhLibString::HasOnlyUnicode(TEXT("Привет")));
	TestFalse(TEXT("Numbers"), UGdhLibString::HasOnlyUnicode(TEXT("12345")));
	TestFalse(TEXT("Special Characters"), UGdhLibString::HasOnlyUnicode(TEXT("!@#$%^")));
	TestTrue(TEXT("Extended ASCII"), UGdhLibString::HasOnlyUnicode(TEXT("ÿüöäß")));

	return true;
}


#endif
