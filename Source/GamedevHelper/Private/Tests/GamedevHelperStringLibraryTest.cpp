// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Kismet/KismetStringLibrary.h"
#include "GamedevHelper.h"
#include "GamedevHelperTester.h"
#include "GamedevHelperStringLibrary.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetRandomStringFromCharsetTest,
	"Plugins.GamedevHelper.Libraries.String.GetRandomStringFromCharset",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetFirstLetterTest,
	"Plugins.GamedevHelper.Libraries.String.GetFirstLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetLastLetterTest,
	"Plugins.GamedevHelper.Libraries.String.GetLastLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FStringLibraryGetRandomStringFromCharsetTest::RunTest(const FString& Parameters)
{
	// contracts
	// must return exactly given size string

	constexpr int32 Seed = 0;
	const auto GetRandomStringTestScenario = [&]()
	{
		for (int32 i = 0; i < 100; ++i)
		{
			const FString RandomString = UGamedevHelperStringLibrary::GetRandomStringFromCharset(i, GamedevHelperConstants::AlphaMixed, Seed);
			if (RandomString.Len() != i)
			{
				const FString Msg = FString::Printf(
					TEXT("Expected %s to have length %d, got %d"),
					*RandomString,
					i,
					RandomString.Len()
				);
				UE_LOG(LogGamedevHelper, Error, TEXT("%s"), *Msg);
				return false;
			}
		}
		
		return true;
	};

	FGamedevHelperTester::ExpectTestScenarioPass(GetRandomStringTestScenario);
	return true;
}

bool FStringLibraryGetFirstLetterTest::RunTest(const FString& Parameters)
{
	// contracts
	// must return empty string if input is empty
	// must return single character case sensitive

	FGamedevHelperTester::ExpectStrEmpty(UGamedevHelperStringLibrary::GetFirstLetter(TEXT("")));

	const auto FirstLetterTestScenario = []()
	{
		for (int32 i = 0; i < 100; ++i)
		{
			const FString RandomString = UGamedevHelperStringLibrary::GetRandomStringFromCharset(
				i,
				GamedevHelperConstants::AlphaMixed + GamedevHelperConstants::Digits,
				i
			);
			const FString ExpectedFirstLetter = UKismetStringLibrary::GetSubstring(RandomString, 0, 1);
			const FString ActualFirstLetter = UGamedevHelperStringLibrary::GetFirstLetter(RandomString);

			if (ActualFirstLetter.Equals(ExpectedFirstLetter, ESearchCase::CaseSensitive) == false)
			{
				const FString Msg = FString::Printf(
					TEXT("Expected FirstLetter of %s, to be %s, got %s"),
					*RandomString,
					*ExpectedFirstLetter,
					*ActualFirstLetter
				);
				UE_LOG(LogGamedevHelper, Error, TEXT("%s"), *Msg);
				return false;
			}
		}

		return true;
	};
	FGamedevHelperTester::ExpectTestScenarioPass(FirstLetterTestScenario);

	const auto FirstLetterManualTestScenario = []()
	{
		// input => expected
		TMap<FString, FString> Tests;
		Tests.Add(TEXT("abc"), TEXT("a"));
		Tests.Add(TEXT("ABC"), TEXT("A"));
		Tests.Add(TEXT("012"), TEXT("0"));
		Tests.Add(TEXT("_-_"), TEXT("_"));
		Tests.Add(TEXT("-_-"), TEXT("-"));
		Tests.Add(TEXT(".png"), TEXT("."));

		for (const auto& Test : Tests)
		{
			const FString Input = Test.Key;
			const FString Actual = UGamedevHelperStringLibrary::GetFirstLetter(Input);
			const FString Expected = Test.Value;

			if (Actual != Expected)
			{
				const FString Msg = FString::Printf(
					TEXT("Expected FirstLetter of %s, to be %s, got %s"),
					*Input,
					*Expected,
					*Actual
				);
				UE_LOG(LogGamedevHelper, Error, TEXT("%s"), *Msg);
				return false;
			}
		}

		return true;
	};
	FGamedevHelperTester::ExpectTestScenarioPass(FirstLetterManualTestScenario);

	return true;
}

bool FStringLibraryGetLastLetterTest::RunTest(const FString& Parameters)
{
	// contracts
	// must return empty string if input is empty
	// must return single character case sensitive

	FGamedevHelperTester::ExpectStrEmpty(UGamedevHelperStringLibrary::GetLastLetter(TEXT("")));

	const auto LastLetterTestScenario = []()
	{
		for (int32 i = 0; i < 100; ++i)
		{
			const FString RandomString = UGamedevHelperStringLibrary::GetRandomStringFromCharset(
				i,
				GamedevHelperConstants::AlphaMixed + GamedevHelperConstants::Digits,
				i
			);
			const FString ExpectedLastLetter = UKismetStringLibrary::GetSubstring(RandomString, RandomString.Len() - 1, 1);
			const FString ActualLastLetter = UGamedevHelperStringLibrary::GetLastLetter(RandomString);

			if (ActualLastLetter.Equals(ExpectedLastLetter, ESearchCase::CaseSensitive) == false)
			{
				const FString Msg = FString::Printf(
					TEXT("Expected LastLetter of %s, to be %s, got %s"),
					*RandomString,
					*ExpectedLastLetter,
					*ActualLastLetter
				);
				UE_LOG(LogGamedevHelper, Error, TEXT("%s"), *Msg);
				return false;
			}
		}

		return true;
	};
	FGamedevHelperTester::ExpectTestScenarioPass(LastLetterTestScenario);

	const auto LastLetterManualTestScenario = []()
	{
		// input => expected
		TMap<FString, FString> Tests;
		Tests.Add(TEXT("abc"), TEXT("c"));
		Tests.Add(TEXT("ABC"), TEXT("C"));
		Tests.Add(TEXT("012"), TEXT("2"));
		Tests.Add(TEXT("_-_"), TEXT("_"));
		Tests.Add(TEXT("-_-"), TEXT("-"));
		Tests.Add(TEXT(".png"), TEXT("g"));

		for (const auto& Test : Tests)
		{
			const FString Input = Test.Key;
			const FString Actual = UGamedevHelperStringLibrary::GetLastLetter(Input);
			const FString Expected = Test.Value;

			if (Actual != Expected)
			{
				const FString Msg = FString::Printf(
					TEXT("Expected LastLetter of %s, to be %s, got %s"),
					*Input,
					*Expected,
					*Actual
				);
				UE_LOG(LogGamedevHelper, Error, TEXT("%s"), *Msg);
				return false;
			}
		}

		return true;
	};
	FGamedevHelperTester::ExpectTestScenarioPass(LastLetterManualTestScenario);

	return true;
}

// todo:ashe23 add 100% test coverage
