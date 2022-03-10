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

bool ContainsOnly(const FString& Input, const FString& Dict, const ESearchCase::Type SearchCase)
{
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
