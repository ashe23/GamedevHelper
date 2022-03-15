// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GamedevHelperStringLibrary.h"
#include "GamedevHelperTester.h"
#include "Kismet/KismetStringLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetFirstLetterTest,
	"Plugins.GamedevHelper.Libraries.String.GetFirstLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FStringLibraryGetFirstLetterTest::RunTest(const FString& Parameters)
{
	// contracts
	// must return empty string if input is empty
	// must return single character case sensitive

	FGamedevHelperTester::ExpectStrEmpty(UGamedevHelperStringLibrary::GetFirstLetter(TEXT("")));

	const auto FirstLetterTestScenario = []()
	{
		const FString RandomString = TEXT("abc"); // todo:ashe23 change to random value
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
