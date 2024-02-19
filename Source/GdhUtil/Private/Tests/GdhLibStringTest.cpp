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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringIntersection, "Gdh.Library.String.Intersection", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringUnion, "Gdh.Library.String.Union", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringDifference, "Gdh.Library.String.Difference", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringSymmetricDifference, "Gdh.Library.String.SymmetricDifference", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringNormalize, "Gdh.Library.String.Normalize", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringTokenize, "Gdh.Library.String.Tokenize", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringPascalCase, "Gdh.Library.String.PascalCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringPascalSnakeCase, "Gdh.Library.String.PascalSnakeCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringUpperCase, "Gdh.Library.String.UpperCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringLowerCase, "Gdh.Library.String.LowerCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringSnakeCase, "Gdh.Library.String.SnakeCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringCamelCase, "Gdh.Library.String.CamelCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibStringKebabCase, "Gdh.Library.String.KebabCase", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

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

bool FGdhLibStringIntersection::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Both Strings Empty"), UGdhLibString::Intersection(TEXT(""), TEXT("")), TEXT(""));
	TestEqual(TEXT("First String Empty"), UGdhLibString::Intersection(TEXT(""), TEXT("abc")), TEXT(""));
	TestEqual(TEXT("Second String Empty"), UGdhLibString::Intersection(TEXT("abc"), TEXT("")), TEXT(""));
	TestEqual(TEXT("No Common Characters"), UGdhLibString::Intersection(TEXT("abc"), TEXT("def")), TEXT(""));
	TestTrue(TEXT("Identical Strings"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("abc"), TEXT("abc")), TEXT("abc")));
	TestTrue(TEXT("All Characters Common"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("abc"), TEXT("bac")), TEXT("abc")));
	TestTrue(TEXT("Some Characters Common #1"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("abc"), TEXT("abd")), TEXT("ab")));
	TestTrue(TEXT("Some Characters Common #2"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("abcdef"), TEXT("f")), TEXT("f")));
	TestTrue(TEXT("Common Characters With Repetitions"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("aabbcc"), TEXT("ab")), TEXT("ab")));
	TestTrue(TEXT("Case Sensitive Test"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("aAbBcC"), TEXT("ABC")), TEXT("ABC")));
	TestTrue(TEXT("Special Characters"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("!@#$"), TEXT("#$%^")), TEXT("#$")));
	TestTrue(TEXT("Unicode Characters #1"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("Привет"), TEXT("вет")), TEXT("вет")));
	TestEqual(TEXT("Unicode Characters #2"), UGdhLibString::Intersection(TEXT("Привет"), TEXT("Ж")), TEXT(""));
	TestEqual(TEXT("Unicode Characters #3"), UGdhLibString::Intersection(TEXT("ППП"), TEXT("п")), TEXT(""));
	TestTrue(TEXT("Extended ASCII"), UGdhLibString::HasOnly(UGdhLibString::Intersection(TEXT("ÿüöäß"), TEXT("äöü")), TEXT("üöä")));
	TestEqual(TEXT("NewLine"), UGdhLibString::Intersection(TEXT("\naa"), TEXT("\n")), TEXT("\n"));

	return true;
}

bool FGdhLibStringUnion::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Both Strings Empty"), UGdhLibString::Union(TEXT(""), TEXT("")), TEXT(""));
	TestEqual(TEXT("First String Empty"), UGdhLibString::Union(TEXT(""), TEXT("abc")), TEXT("abc"));
	TestEqual(TEXT("Second String Empty"), UGdhLibString::Union(TEXT("abc"), TEXT("")), TEXT("abc"));
	TestEqual(TEXT("No Common Characters"), UGdhLibString::Union(TEXT("abc"), TEXT("def")), TEXT("abcdef"));
	TestTrue(TEXT("Identical Strings"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("abc"), TEXT("abc")), TEXT("abc")));
	TestTrue(TEXT("All Characters Common"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("abc"), TEXT("bac")), TEXT("abc")));
	TestTrue(TEXT("Some Characters Common #1"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("abc"), TEXT("abd")), TEXT("abcd")));
	TestTrue(TEXT("Some Characters Common #2"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("abcdef"), TEXT("f")), TEXT("abcdef")));
	TestTrue(TEXT("Common Characters With Repetitions"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("aabbcc"), TEXT("ab")), TEXT("abc")));
	TestTrue(TEXT("Case Sensitive Test"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("aAbBcC"), TEXT("ABC")), TEXT("AaBbCc")));
	TestTrue(TEXT("Special Characters"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("!@#$"), TEXT("#$%^")), TEXT("!@#$%^")));
	TestTrue(TEXT("Unicode Characters #1"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("Привет"), TEXT("вет")), TEXT("Привет")));
	TestEqual(TEXT("Unicode Characters #2"), UGdhLibString::Union(TEXT("Привет"), TEXT("Ж")), TEXT("ПриветЖ"));
	TestEqual(TEXT("Unicode Characters #3"), UGdhLibString::Union(TEXT("ППП"), TEXT("п")), TEXT("Пп"));
	TestTrue(TEXT("Extended ASCII"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("ÿüöäß"), TEXT("äöü")), TEXT("ÿüöäß")));
	TestTrue(TEXT("NewLine"), UGdhLibString::HasOnly(UGdhLibString::Union(TEXT("\nab"), TEXT("\n")), TEXT("\nabn")));

	return true;
}

bool FGdhLibStringDifference::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Both Strings Empty"), UGdhLibString::Difference(TEXT(""), TEXT("")), TEXT(""));
	TestEqual(TEXT("First String Empty"), UGdhLibString::Difference(TEXT(""), TEXT("abc")), TEXT(""));
	TestEqual(TEXT("Second String Empty"), UGdhLibString::Difference(TEXT("abc"), TEXT("")), TEXT("abc"));
	TestEqual(TEXT("No Common Characters"), UGdhLibString::Difference(TEXT("abc"), TEXT("def")), TEXT("abc"));
	TestEqual(TEXT("Identical Strings"), UGdhLibString::Difference(TEXT("abc"), TEXT("abc")), TEXT(""));
	TestEqual(TEXT("All Characters Common"), UGdhLibString::Difference(TEXT("abc"), TEXT("cba")), TEXT(""));
	TestEqual(TEXT("Some Characters Common #1"), UGdhLibString::Difference(TEXT("abc"), TEXT("ade")), TEXT("bc"));
	TestEqual(TEXT("Some Characters Common #2"), UGdhLibString::Difference(TEXT("abcdef"), TEXT("defxyz")), TEXT("abc"));
	TestEqual(TEXT("Common Characters With Repetitions"), UGdhLibString::Difference(TEXT("aabbcc"), TEXT("bccdde")), TEXT("a"));
	TestEqual(TEXT("Case Sensitive Test"), UGdhLibString::Difference(TEXT("aAbBcC"), TEXT("ABC")), TEXT("abc"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::Difference(TEXT("!@#$"), TEXT("#$%^")), TEXT("!@"));
	TestEqual(TEXT("Unicode Characters"), UGdhLibString::Difference(TEXT("Привет"), TEXT("етмир")), TEXT("Пв"));
	TestEqual(TEXT("Extended ASCII"), UGdhLibString::Difference(TEXT("ÿüöäß"), TEXT("äöü")), TEXT("ÿß"));

	return true;
}


bool FGdhLibStringSymmetricDifference::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Both Strings Empty"), UGdhLibString::SymmetricDifference(TEXT(""), TEXT("")), TEXT(""));
	TestEqual(TEXT("First String Empty"), UGdhLibString::SymmetricDifference(TEXT(""), TEXT("abc")), TEXT("abc"));
	TestEqual(TEXT("Second String Empty"), UGdhLibString::SymmetricDifference(TEXT("abc"), TEXT("")), TEXT("abc"));
	TestEqual(TEXT("No Common Characters"), UGdhLibString::SymmetricDifference(TEXT("abc"), TEXT("def")), TEXT("abcdef"));
	TestEqual(TEXT("Identical Strings"), UGdhLibString::SymmetricDifference(TEXT("abc"), TEXT("abc")), TEXT(""));
	TestEqual(TEXT("All Characters Common"), UGdhLibString::SymmetricDifference(TEXT("abc"), TEXT("bac")), TEXT(""));
	TestTrue(TEXT("Some Characters Common #1"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("abc"), TEXT("abd")), TEXT("dc")));
	TestTrue(TEXT("Some Characters Common #2"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("abcdef"), TEXT("f")), TEXT("abcde")));
	TestTrue(TEXT("Common Characters With Repetitions"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("aabbcc"), TEXT("ab")), TEXT("c")));
	TestEqual(TEXT("Case Sensitive Test"), UGdhLibString::SymmetricDifference(TEXT("AaBbCc"), TEXT("ABC")), TEXT("abc"));
	TestTrue(TEXT("Special Characters"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("!@#$"), TEXT("#$%^")), TEXT("%@!^")));
	TestTrue(TEXT("Unicode Characters #1"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("Привет"), TEXT("вет")), TEXT("При")));
	TestEqual(TEXT("Unicode Characters #2"), UGdhLibString::SymmetricDifference(TEXT("Привет"), TEXT("Ж")), TEXT("ПриветЖ"));
	TestEqual(TEXT("Unicode Characters #3"), UGdhLibString::SymmetricDifference(TEXT("ППП"), TEXT("п")), TEXT("Пп"));
	TestTrue(TEXT("Extended ASCII"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("ÿüöäß"), TEXT("äöü")), TEXT("ÿß")));
	TestTrue(TEXT("NewLine"), UGdhLibString::HasOnly(UGdhLibString::SymmetricDifference(TEXT("\nab"), TEXT("\n")), TEXT("ab")));

	return true;
}

bool FGdhLibStringNormalize::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty string"), UGdhLibString::Normalize(TEXT("")), TEXT(""));
	TestEqual(TEXT("Ansi chars #1"), UGdhLibString::Normalize(TEXT("abc")), TEXT("abc"));
	TestEqual(TEXT("Ansi chars #2"), UGdhLibString::Normalize(TEXT("ABC")), TEXT("ABC"));
	TestEqual(TEXT("Ansi chars #3"), UGdhLibString::Normalize(TEXT("aaa___bbb___ccc")), TEXT("aaa_bbb_ccc"));
	TestEqual(TEXT("Ansi chars #4"), UGdhLibString::Normalize(TEXT("-aa-bb-cc--01-")), TEXT("aa_bb_cc_01"));
	TestEqual(TEXT("Must Not Start With Underscore"), UGdhLibString::Normalize(TEXT("_aBc")), TEXT("aBc"));
	TestEqual(TEXT("Must Not End With Underscore"), UGdhLibString::Normalize(TEXT("aBc_")), TEXT("aBc"));
	TestEqual(TEXT("Must Not Start or End With Underscore"), UGdhLibString::Normalize(TEXT("_aBc_")), TEXT("aBc"));
	TestEqual(TEXT("Trailing underscores #1"), UGdhLibString::Normalize(TEXT("__aBc__")), TEXT("aBc"));
	TestEqual(TEXT("Trailing underscores #2"), UGdhLibString::Normalize(TEXT("__aB__c__")), TEXT("aB_c"));
	TestEqual(TEXT("Trailing underscores #3"), UGdhLibString::Normalize(TEXT("__aB__c01__")), TEXT("aB_c01"));
	TestEqual(TEXT("Special Chars #1"), UGdhLibString::Normalize(GdhConstants::SpecialChars + TEXT("aBc")), TEXT("aBc"));
	TestEqual(TEXT("Special Chars #2"), UGdhLibString::Normalize(TEXT("aBc") + GdhConstants::SpecialChars), TEXT("aBc"));
	TestEqual(TEXT("Special Chars #3"), UGdhLibString::Normalize(TEXT("-A__-b-__C__")), TEXT("A_b_C"));
	TestEqual(TEXT("Unicode Chars #1"), UGdhLibString::Normalize(TEXT("Привет")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #2"), UGdhLibString::Normalize(TEXT("_Привет")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #3"), UGdhLibString::Normalize(TEXT("Привет_")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #4"), UGdhLibString::Normalize(TEXT("_Привет_")), TEXT("Привет"));
	TestEqual(TEXT("Extended ASCII"), UGdhLibString::Normalize(TEXT("-_ÿüöä+ß_-")), TEXT("ÿüöä_ß"));
	TestEqual(TEXT("NewLine"), UGdhLibString::Normalize(TEXT("\nab")), TEXT("ab"));

	return true;
}

bool FGdhLibStringTokenize::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty string"), UGdhLibString::Tokenize(TEXT("")), TEXT(""));
	TestEqual(TEXT("Ansi chars #1"), UGdhLibString::Tokenize(TEXT("abc")), TEXT("abc"));
	TestEqual(TEXT("Ansi chars #2"), UGdhLibString::Tokenize(TEXT("ABC")), TEXT("abc"));
	TestEqual(TEXT("Ansi chars #3"), UGdhLibString::Tokenize(TEXT("aaa___bbb___ccc")), TEXT("aaa_bbb_ccc"));
	TestEqual(TEXT("Ansi chars #4"), UGdhLibString::Tokenize(TEXT("-aa-bb-cc--01-")), TEXT("aa_bb_cc_01"));
	TestEqual(TEXT("Ansi chars #5"), UGdhLibString::Tokenize(TEXT("PascalCase01")), TEXT("pascal_case01"));
	TestEqual(TEXT("Ansi chars #6"), UGdhLibString::Tokenize(TEXT("01_my-string")), TEXT("01_my_string"));
	TestEqual(TEXT("Ansi chars #7"), UGdhLibString::Tokenize(TEXT("TextureUV01")), TEXT("texture_uv01"));
	TestEqual(TEXT("Ansi chars #8"), UGdhLibString::Tokenize(TEXT("TextureUV_001")), TEXT("texture_uv_001"));
	TestEqual(TEXT("Must Not Start With Underscore"), UGdhLibString::Tokenize(TEXT("_aBc")), TEXT("a_bc"));
	TestEqual(TEXT("Must Not End With Underscore"), UGdhLibString::Tokenize(TEXT("aBc_")), TEXT("a_bc"));
	TestEqual(TEXT("Must Not Start or End With Underscore"), UGdhLibString::Tokenize(TEXT("_aBc_")), TEXT("a_bc"));
	TestEqual(TEXT("Trailing underscores #1"), UGdhLibString::Tokenize(TEXT("__aBc__")), TEXT("a_bc"));
	TestEqual(TEXT("Trailing underscores #2"), UGdhLibString::Tokenize(TEXT("__aB__c__")), TEXT("a_b_c"));
	TestEqual(TEXT("Trailing underscores #3"), UGdhLibString::Tokenize(TEXT("__aB__c01__")), TEXT("a_b_c01"));
	TestEqual(TEXT("Special Chars #1"), UGdhLibString::Tokenize(GdhConstants::SpecialChars + TEXT("aBc")), TEXT("a_bc"));
	TestEqual(TEXT("Special Chars #2"), UGdhLibString::Tokenize(TEXT("aBc") + GdhConstants::SpecialChars), TEXT("a_bc"));
	TestEqual(TEXT("Special Chars #3"), UGdhLibString::Tokenize(TEXT("-A__-b-__C__")), TEXT("a_b_c"));
	TestEqual(TEXT("Unicode Chars #1"), UGdhLibString::Tokenize(TEXT("Привет")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #2"), UGdhLibString::Tokenize(TEXT("_Привет")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #3"), UGdhLibString::Tokenize(TEXT("Привет_")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #4"), UGdhLibString::Tokenize(TEXT("_Привет_")), TEXT("Привет"));
	TestEqual(TEXT("Unicode Chars #5"), UGdhLibString::Tokenize(TEXT("_Привет_01")), TEXT("Привет_01"));
	TestEqual(TEXT("Unicode Chars #6"), UGdhLibString::Tokenize(TEXT("001_Привет_01")), TEXT("001_Привет_01"));
	TestEqual(TEXT("Extended ASCII"), UGdhLibString::Tokenize(TEXT("-_ÿüöä+ß_-")), TEXT("ÿüöä_ß"));
	TestEqual(TEXT("NewLine"), UGdhLibString::Tokenize(TEXT("\nab")), TEXT("ab"));

	return true;
}

bool FGdhLibStringPascalCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToPascalCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToPascalCase(TEXT("word")), TEXT("Word"));
	TestEqual(TEXT("Single Word Uppercase #1"), UGdhLibString::ConvertToPascalCase(TEXT("WORD")), TEXT("Word"));
	TestEqual(TEXT("Single Word Uppercase #2"), UGdhLibString::ConvertToPascalCase(TEXT("MyTexture23")), TEXT("MyTexture23"));
	TestEqual(TEXT("Single Word Uppercase #3"), UGdhLibString::ConvertToPascalCase(TEXT("MyTexture_23")), TEXT("MyTexture23"));
	TestEqual(TEXT("Multiple Words Lowercase"), UGdhLibString::ConvertToPascalCase(TEXT("multiple words lowercase")), TEXT("MultipleWordsLowercase"));
	TestEqual(TEXT("Multiple Words Uppercase"), UGdhLibString::ConvertToPascalCase(TEXT("MULTIPLE WORDS UPPERCASE")), TEXT("MultipleWordsUppercase"));
	TestEqual(TEXT("Mixed Case Words"), UGdhLibString::ConvertToPascalCase(TEXT("Mixed Case Words")), TEXT("MixedCaseWords"));
	TestEqual(TEXT("Words With Underscores"), UGdhLibString::ConvertToPascalCase(TEXT("words_with_underscores")), TEXT("WordsWithUnderscores"));
	TestEqual(TEXT("Words With Hyphens"), UGdhLibString::ConvertToPascalCase(TEXT("words-with-hyphens")), TEXT("WordsWithHyphens"));
	TestEqual(TEXT("Words With Mixed Separators"), UGdhLibString::ConvertToPascalCase(TEXT("words_with-mixed separators")), TEXT("WordsWithMixedSeparators"));
	TestEqual(TEXT("Starting With Separator"), UGdhLibString::ConvertToPascalCase(TEXT("-starting with separator")), TEXT("StartingWithSeparator"));
	TestEqual(TEXT("Ending With Separator"), UGdhLibString::ConvertToPascalCase(TEXT("ending with separator_")), TEXT("EndingWithSeparator"));
	TestEqual(TEXT("Numbers In String"), UGdhLibString::ConvertToPascalCase(TEXT("numbers 2 in 4 string")), TEXT("Numbers2In4String"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::ConvertToPascalCase(TEXT("special!@# characters")), TEXT("SpecialCharacters"));
	TestEqual(TEXT("Unicode Characters"), UGdhLibString::ConvertToPascalCase(TEXT("unicode 字符")), TEXT("Unicode字符"));
	TestEqual(TEXT("Mixed Case With Underscores And Hyphens"), UGdhLibString::ConvertToPascalCase(TEXT("mixed_Case-with_underscores_and-hyphens")), TEXT("MixedCaseWithUnderscoresAndHyphens"));


	return true;
}

bool FGdhLibStringPascalSnakeCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("word")), TEXT("Word"));
	TestEqual(TEXT("Single Word Uppercase"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("WORD")), TEXT("Word"));
	TestEqual(TEXT("Ansi #1"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("my_material23")), TEXT("My_Material23"));
	TestEqual(TEXT("Ansi #2"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("my_material_23")), TEXT("My_Material_23"));
	TestEqual(TEXT("Ansi #3"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("my_material_23")), TEXT("My_Material_23"));
	TestEqual(TEXT("Ansi #4"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("01_NAME_Brick023")), TEXT("01_Name_Brick023"));
	TestEqual(TEXT("Ansi #5"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("%^&aaa*(")), TEXT("aaa"));
	TestEqual(TEXT("Ansi #6"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("my-texture---023")), TEXT("My_Texture_023"));
	TestEqual(TEXT("Unicode #1"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("привет23")), TEXT("привет23"));
	TestEqual(TEXT("Unicode #2"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("привет_23")), TEXT("привет_23"));
	TestEqual(TEXT("Unicode #2"), UGdhLibString::ConvertToPascalSnakeCase(TEXT("Привет-23_АБВ")), TEXT("Привет_23_АБВ"));

	return true;
}


bool FGdhLibStringUpperCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToUpperCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToUpperCase(TEXT("word")), TEXT("WORD"));
	TestEqual(TEXT("Single Word Uppercase"), UGdhLibString::ConvertToUpperCase(TEXT("WORD")), TEXT("WORD"));
	TestEqual(TEXT("Multiple Words Lowercase"), UGdhLibString::ConvertToUpperCase(TEXT("multiple words lowercase")), TEXT("MULTIPLE_WORDS_LOWERCASE"));
	TestEqual(TEXT("Multiple Words Mixed Case"), UGdhLibString::ConvertToUpperCase(TEXT("Multiple Words Mixed Case")), TEXT("MULTIPLE_WORDS_MIXED_CASE"));
	TestEqual(TEXT("Words With Underscores"), UGdhLibString::ConvertToUpperCase(TEXT("words_with_underscores")), TEXT("WORDS_WITH_UNDERSCORES"));
	TestEqual(TEXT("Words With Hyphens"), UGdhLibString::ConvertToUpperCase(TEXT("words-with-hyphens")), TEXT("WORDS_WITH_HYPHENS"));
	TestEqual(TEXT("Numbers In String"), UGdhLibString::ConvertToUpperCase(TEXT("numbers 123 in string")), TEXT("NUMBERS_123_IN_STRING"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::ConvertToUpperCase(TEXT("special!@# characters")), TEXT("SPECIAL_CHARACTERS"));
	TestEqual(TEXT("Mixed Case With Underscores And Hyphens"), UGdhLibString::ConvertToUpperCase(TEXT("mixed_Case-with_underscores_and-hyphens")), TEXT("MIXED_CASE_WITH_UNDERSCORES_AND_HYPHENS"));

	return true;
}

bool FGdhLibStringLowerCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToLowerCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Uppercase"), UGdhLibString::ConvertToLowerCase(TEXT("WORD")), TEXT("word"));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToLowerCase(TEXT("word")), TEXT("word"));
	TestEqual(TEXT("Multiple Words Uppercase"), UGdhLibString::ConvertToLowerCase(TEXT("MULTIPLE WORDS UPPERCASE")), TEXT("multiple_words_uppercase"));
	TestEqual(TEXT("Multiple Words Mixed Case"), UGdhLibString::ConvertToLowerCase(TEXT("Multiple Words Mixed Case")), TEXT("multiple_words_mixed_case"));
	TestEqual(TEXT("Words With Underscores"), UGdhLibString::ConvertToLowerCase(TEXT("WORDS_WITH_UNDERSCORES")), TEXT("words_with_underscores"));
	TestEqual(TEXT("Words With Hyphens"), UGdhLibString::ConvertToLowerCase(TEXT("WORDS-WITH-HYPHENS")), TEXT("words_with_hyphens"));
	TestEqual(TEXT("Numbers In String"), UGdhLibString::ConvertToLowerCase(TEXT("NUMBERS 123 IN STRING")), TEXT("numbers_123_in_string"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::ConvertToLowerCase(TEXT("SPECIAL!@# CHARACTERS")), TEXT("special_characters"));
	TestEqual(TEXT("Mixed Case With Underscores And Hyphens"), UGdhLibString::ConvertToLowerCase(TEXT("Mixed_Case-With_Underscores_And-Hyphens")), TEXT("mixed_case_with_underscores_and_hyphens"));

	return true;
}

bool FGdhLibStringSnakeCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToSnakeCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToSnakeCase(TEXT("word")), TEXT("word"));
	TestEqual(TEXT("Single Word Uppercase"), UGdhLibString::ConvertToSnakeCase(TEXT("WORD")), TEXT("word"));
	TestEqual(TEXT("Multiple Words Lowercase"), UGdhLibString::ConvertToSnakeCase(TEXT("multiple words lowercase")), TEXT("multiple_words_lowercase"));
	TestEqual(TEXT("Multiple Words Uppercase"), UGdhLibString::ConvertToSnakeCase(TEXT("MULTIPLE WORDS UPPERCASE")), TEXT("multiple_words_uppercase"));
	TestEqual(TEXT("Mixed Case Words"), UGdhLibString::ConvertToSnakeCase(TEXT("Mixed Case Words")), TEXT("mixed_case_words"));
	TestEqual(TEXT("Words With Underscores"), UGdhLibString::ConvertToSnakeCase(TEXT("words_with_underscores")), TEXT("words_with_underscores"));
	TestEqual(TEXT("Words With Hyphens"), UGdhLibString::ConvertToSnakeCase(TEXT("words-with-hyphens")), TEXT("words_with_hyphens"));
	TestEqual(TEXT("Words With Mixed Separators"), UGdhLibString::ConvertToSnakeCase(TEXT("words with_mixed-separators")), TEXT("words_with_mixed_separators"));
	TestEqual(TEXT("Starting With Separator"), UGdhLibString::ConvertToSnakeCase(TEXT("-starting with separator")), TEXT("starting_with_separator"));
	TestEqual(TEXT("Ending With Separator"), UGdhLibString::ConvertToSnakeCase(TEXT("ending with separator_")), TEXT("ending_with_separator"));
	TestEqual(TEXT("Numbers In String"), UGdhLibString::ConvertToSnakeCase(TEXT("numbers 2 in 4 string")), TEXT("numbers_2_in_4_string"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::ConvertToSnakeCase(TEXT("special!@# characters")), TEXT("special_characters"));
	TestEqual(TEXT("Consecutive Non-Alphanumeric"), UGdhLibString::ConvertToSnakeCase(TEXT("consecutive   non-alphanumeric!characters")), TEXT("consecutive_non_alphanumeric_characters"));
	TestEqual(TEXT("Mixed Case With Underscores And Hyphens"), UGdhLibString::ConvertToSnakeCase(TEXT("Mixed_Case-With_Underscores_And-Hyphens")), TEXT("mixed_case_with_underscores_and_hyphens"));

	return true;
}

bool FGdhLibStringCamelCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToCamelCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToCamelCase(TEXT("word")), TEXT("word"));
	TestEqual(TEXT("Single Word Uppercase"), UGdhLibString::ConvertToCamelCase(TEXT("WORD")), TEXT("word"));
	TestEqual(TEXT("Multiple Words Lowercase"), UGdhLibString::ConvertToCamelCase(TEXT("multiple words lowercase")), TEXT("multipleWordsLowercase"));
	TestEqual(TEXT("Multiple Words Uppercase"), UGdhLibString::ConvertToCamelCase(TEXT("MULTIPLE WORDS UPPERCASE")), TEXT("multipleWordsUppercase"));
	TestEqual(TEXT("Mixed Case Words"), UGdhLibString::ConvertToCamelCase(TEXT("Mixed Case Words")), TEXT("mixedCaseWords"));
	TestEqual(TEXT("Words With Underscores"), UGdhLibString::ConvertToCamelCase(TEXT("words_with_underscores")), TEXT("wordsWithUnderscores"));
	TestEqual(TEXT("Words With Hyphens"), UGdhLibString::ConvertToCamelCase(TEXT("words-with-hyphens")), TEXT("wordsWithHyphens"));
	TestEqual(TEXT("Numbers In String"), UGdhLibString::ConvertToCamelCase(TEXT("numbers 2 in 4 string")), TEXT("numbers2In4String"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::ConvertToCamelCase(TEXT("special!@# characters")), TEXT("specialCharacters"));
	TestEqual(TEXT("Mixed Case With Underscores And Hyphens"), UGdhLibString::ConvertToCamelCase(TEXT("Mixed_Case-With_Underscores_And-Hyphens")), TEXT("mixedCaseWithUnderscoresAndHyphens"));

	return true;
}

bool FGdhLibStringKebabCase::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibString::ConvertToKebabCase(TEXT("")), TEXT(""));
	TestEqual(TEXT("Single Word Lowercase"), UGdhLibString::ConvertToKebabCase(TEXT("word")), TEXT("word"));
	TestEqual(TEXT("Single Word Uppercase"), UGdhLibString::ConvertToKebabCase(TEXT("WORD")), TEXT("word"));
	TestEqual(TEXT("Multiple Words Lowercase"), UGdhLibString::ConvertToKebabCase(TEXT("multiple words lowercase")), TEXT("multiple-words-lowercase"));
	TestEqual(TEXT("Multiple Words Uppercase"), UGdhLibString::ConvertToKebabCase(TEXT("MULTIPLE WORDS UPPERCASE")), TEXT("multiple-words-uppercase"));
	TestEqual(TEXT("Mixed Case Words"), UGdhLibString::ConvertToKebabCase(TEXT("Mixed Case Words")), TEXT("mixed-case-words"));
	TestEqual(TEXT("Words With Underscores"), UGdhLibString::ConvertToKebabCase(TEXT("words_with_underscores")), TEXT("words-with-underscores"));
	TestEqual(TEXT("Words With Hyphens"), UGdhLibString::ConvertToKebabCase(TEXT("words-with-hyphens")), TEXT("words-with-hyphens"));
	TestEqual(TEXT("Numbers In String"), UGdhLibString::ConvertToKebabCase(TEXT("numbers 2 in 4 string")), TEXT("numbers-2-in-4-string"));
	TestEqual(TEXT("Special Characters"), UGdhLibString::ConvertToKebabCase(TEXT("special!@# characters")), TEXT("special-characters"));
	TestEqual(TEXT("Mixed Case With Underscores And Hyphens"), UGdhLibString::ConvertToKebabCase(TEXT("Mixed_Case-With_Underscores_And-Hyphens")), TEXT("mixed-case-with-underscores-and-hyphens"));

	return true;
}

#endif
