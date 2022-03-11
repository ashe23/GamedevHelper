// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Fuzzer.h"
#include "TestRunner.h"
#include "Kismet/KismetStringLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FFuzzerGetRandomStringTest,
	"Plugins.GamedevHelper.Tester.Fuzzer.GetRandomString",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FFuzzerGetRandomStringFormatTest,
	"Plugins.GamedevHelper.Tester.Fuzzer.GetRandomStringFormat",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool ContainsOnly(const FString& Input, const FString& Dict, const ESearchCase::Type SearchCase)
{
	if (Input.IsEmpty() || Dict.IsEmpty()) return false;

	for (int32 i = 0; i < Input.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(Input, i, 1);
		if (!Dict.Contains(SingleChar, SearchCase))
		{
			return false;
		}
	}

	return true;
}

bool Contains(const FString& Input, const FString& Dict, const ESearchCase::Type SearchCase)
{
	if (Input.IsEmpty() || Dict.IsEmpty()) return false;

	for (int32 i = 0; i < Input.Len(); ++i)
	{
		const auto SingleChar = UKismetStringLibrary::GetSubstring(Input, i, 1);
		if (Dict.Contains(SingleChar, SearchCase))
		{
			return true;
		}
	}

	return false;
}

bool FFuzzerGetRandomStringTest::RunTest(const FString& Parameters)
{
	FTestRunner TestRunner;
	const FFuzzer Fuzzer;

	// <0 len string or empty dictionary must return empty string
	const auto EmptyStringZero = Fuzzer.GetRandomString(0, TEXT(""));
	const auto EmptyStringNegative = Fuzzer.GetRandomString(-1, TEXT(""));
	TestRunner.ExpectTrue(EmptyStringZero.IsEmpty(), EmptyStringZero, TEXT("EmptyStringZero"));
	TestRunner.ExpectTrue(EmptyStringNegative.IsEmpty(), EmptyStringNegative, TEXT("EmptyStringNegative"));

	// random length string must be correct length
	for (int32 i = 1; i <= 100; ++i)
	{
		const auto RandomLengthString = Fuzzer.GetRandomString(i, Fuzzer::All);
		TestRunner.ExpectTrue(RandomLengthString.Len() == i, RandomLengthString, TEXT("StringLength"));
	}

	// strings must contain only specified characters from dictionary
	const auto AlphaString = Fuzzer.GetRandomString(10, Fuzzer::Alpha);
	const auto DigitString = Fuzzer.GetRandomString(10, Fuzzer::Digits);
	const auto SpecialString = Fuzzer.GetRandomString(10, Fuzzer::Specials);
	const auto MixedString = Fuzzer.GetRandomString(10, Fuzzer::All);

	TestRunner.ExpectTrue(ContainsOnly(AlphaString, Fuzzer::Alpha, ESearchCase::CaseSensitive), AlphaString, TEXT("DictionaryAlpha"));
	TestRunner.ExpectTrue(ContainsOnly(DigitString, Fuzzer::Digits, ESearchCase::CaseSensitive), DigitString, TEXT("DictionaryDigits"));
	TestRunner.ExpectTrue(ContainsOnly(SpecialString, Fuzzer::Specials, ESearchCase::CaseSensitive), SpecialString, TEXT("DictionarySpecials"));
	TestRunner.ExpectFalse(ContainsOnly(MixedString, TEXT(".[]"), ESearchCase::CaseSensitive), MixedString, TEXT("DictionaryInvalid"));

	return TestRunner.GetResult();
}

bool FFuzzerGetRandomStringFormatTest::RunTest(const FString& Parameters)
{
	FTestRunner TestRunner;
	const FFuzzer Fuzzer;
	constexpr int32 DefaultStringLen = 10;

	// <0 len string or string format none must return empty string
	const auto EmptyStringZero = Fuzzer.GetRandomString(0, Fuzzer::EStringFormat::None);
	const auto EmptyStringNegative = Fuzzer.GetRandomString(-1, Fuzzer::EStringFormat::None);
	TestRunner.ExpectTrue(EmptyStringZero.IsEmpty(), EmptyStringZero, TEXT("EmptyStringZero"));
	TestRunner.ExpectTrue(EmptyStringNegative.IsEmpty(), EmptyStringNegative, TEXT("EmptyStringNegative"));

	// string must contain only lower case letters
	const auto LowerCaseString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::Lower);
	TestRunner.ExpectTrue(ContainsOnly(LowerCaseString, Fuzzer::Alpha, ESearchCase::CaseSensitive), LowerCaseString, TEXT("LowerCase"));

	// string must contain only upper case letters
	const auto UpperCaseString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::Upper);
	TestRunner.ExpectTrue(ContainsOnly(UpperCaseString, Fuzzer::Alpha.ToUpper(), ESearchCase::CaseSensitive), UpperCaseString, TEXT("UpperCase"));

	// string must contain only alphabet letters in any case
	const auto MixedCaseString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::Mixed);
	TestRunner.ExpectTrue(ContainsOnly(MixedCaseString, Fuzzer::Alpha + Fuzzer::Alpha.ToUpper(), ESearchCase::IgnoreCase), MixedCaseString, TEXT("MixedCase"));

	// string must contain only digits
	const auto DigitString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::Digit);
	TestRunner.ExpectTrue(ContainsOnly(DigitString, Fuzzer::Digits, ESearchCase::IgnoreCase), DigitString, TEXT("Digits: Must contain only digits"));

	// string must contain only special chars
	const auto SpecialsString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::Special);
	TestRunner.ExpectTrue(ContainsOnly(SpecialsString, Fuzzer::Specials, ESearchCase::IgnoreCase), SpecialsString, TEXT("Specials: Must contain only special chars"));

	// string first letter must be upper case others lower case
	const auto StartUpperString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::StartUpper);
	const auto StartUpperFirstLetter = UKismetStringLibrary::GetSubstring(StartUpperString, 0, 1);
	const auto StartUpperFirstLetterRemoved = StartUpperString.RightChop(1);
	TestRunner.ExpectTrue(ContainsOnly(StartUpperFirstLetter, Fuzzer::Alpha.ToUpper(), ESearchCase::CaseSensitive), StartUpperFirstLetter, TEXT("StartUpper: Must be upper case"));
	TestRunner.ExpectTrue(ContainsOnly(StartUpperFirstLetterRemoved, Fuzzer::Alpha, ESearchCase::CaseSensitive), StartUpperFirstLetterRemoved, TEXT("StartUpper: Must be lower case"));

	// string last letter must be upper case other lower case
	const auto EndUpperString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::EndUpper);
	const auto EndUpperStringLastLetter = EndUpperString.RightChop(EndUpperString.Len() - 1);
	const auto EndUpperLastLetterRemoved = EndUpperString.LeftChop(1);
	TestRunner.ExpectTrue(ContainsOnly(EndUpperStringLastLetter, Fuzzer::Alpha.ToUpper(), ESearchCase::CaseSensitive), EndUpperStringLastLetter, TEXT("EndUpper: Must be upper case"));
	TestRunner.ExpectTrue(ContainsOnly(EndUpperLastLetterRemoved, Fuzzer::Alpha, ESearchCase::CaseSensitive), EndUpperLastLetterRemoved, TEXT("EndUpper: Must be lower case"));

	// string first letter must be lower case other upper case
	const auto StartLowerString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::StartLower);
	const auto StartLowerFirstLetter = UKismetStringLibrary::GetSubstring(StartLowerString, 0, 1);
	const auto StartLowerFirstLetterRemoved = StartLowerString.RightChop(1);
	TestRunner.ExpectTrue(ContainsOnly(StartLowerFirstLetter, Fuzzer::Alpha, ESearchCase::CaseSensitive), StartLowerFirstLetter, TEXT("StartLower: Must be lower case"));
	TestRunner.ExpectTrue(ContainsOnly(StartLowerFirstLetterRemoved, Fuzzer::Alpha.ToUpper(), ESearchCase::CaseSensitive), StartLowerFirstLetterRemoved, TEXT("StartLower: Must be upper case"));

	// string last letter must be lower case other upper case
	const auto EndLowerString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::EndLower);
	const auto EndLowerLastLetter = EndLowerString.RightChop(EndLowerString.Len() - 1);
	const auto EndLowerLastLetterRemoved = EndLowerString.LeftChop(1);
	TestRunner.ExpectTrue(ContainsOnly(EndLowerLastLetter, Fuzzer::Alpha, ESearchCase::CaseSensitive), EndLowerLastLetter, TEXT("EndLower: Must be lower case"));
	TestRunner.ExpectTrue(ContainsOnly(EndLowerLastLetterRemoved, Fuzzer::Alpha.ToUpper(), ESearchCase::CaseSensitive), EndLowerLastLetterRemoved, TEXT("EndLower: Must be upper case"));

	// string must be in PascalCase
	// must start with Upper case
	// must contain only alpha + digits
	// must not contain Upper case letter before any digit or digit sequence
	const auto PascalCaseString = Fuzzer.GetRandomString(DefaultStringLen, Fuzzer::EStringFormat::PascalCase);
	const auto PascalCaseFirstLetter = UKismetStringLibrary::GetSubstring(PascalCaseString, 0, 1);
	TestRunner.ExpectTrue(ContainsOnly(PascalCaseFirstLetter, Fuzzer::Alpha.ToUpper(), ESearchCase::CaseSensitive), PascalCaseFirstLetter, TEXT("PascalCase: First letter must be upper case"));
	TestRunner.ExpectTrue(ContainsOnly(PascalCaseString, Fuzzer::Alpha + Fuzzer::Alpha.ToUpper() + Fuzzer::Digits, ESearchCase::CaseSensitive), PascalCaseString,TEXT("PascalCase: Must contain only alpha and digits"));

	// string must in Pascal_Snake_Case
	// must start with Upper case
	// must contain only alpha + digits + underscore
	// underscore must be before every Upper Case letter and any digit or digit sequence

	return TestRunner.GetResult();
}