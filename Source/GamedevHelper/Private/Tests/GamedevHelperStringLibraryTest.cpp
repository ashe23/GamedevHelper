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
	FStringLibraryIntersectionTest,
	"Plugins.GamedevHelper.Libraries.String.Intersection",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryUnionTest,
	"Plugins.GamedevHelper.Libraries.String.Union",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibrarySymmetricDifferenceTest,
	"Plugins.GamedevHelper.Libraries.String.SymmetricDifference",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryDifferenceTest,
	"Plugins.GamedevHelper.Libraries.String.Difference",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryIsSubSetTest,
	"Plugins.GamedevHelper.Libraries.String.IsSubSet",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryRegexReplaceTest,
	"Plugins.GamedevHelper.Libraries.String.RegexReplace",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FStringLibraryGetRandomStringFromCharsetTest::RunTest(const FString& Parameters)
{
	// contracts
	// must return exactly given size string
	// must contain only characters from given charset

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

			if (!RandomString.IsEmpty())
			{
				for (int32 j = 0; j < RandomString.Len(); ++j)
				{
					const FString SingleLetter = UKismetStringLibrary::GetSubstring(RandomString, j, 1);
					if (!GamedevHelperConstants::AlphaMixed.Contains(SingleLetter, ESearchCase::CaseSensitive))
					{
						UE_LOG(LogGamedevHelper, Error, TEXT("Expected charset is '%s', got '%s' char"), *GamedevHelperConstants::AlphaMixed,
						       *SingleLetter);
						return false;
					}
				}
			}
		}

		return true;
	};

	FGamedevHelperTester::ExpectTestScenarioPass(GetRandomStringTestScenario);
	return true;
}

bool FStringLibraryIntersectionTest::RunTest(const FString& Parameters)
{
	// https://en.wikipedia.org/wiki/Venn_diagram - Intersection
	
	struct FTestCase
	{
		FString CharsetA;
		FString CharsetB;
		FString Expected;
	};
	
	const auto Scenario = []()
	{
		TArray<FTestCase> Tests;
		Tests.Add(FTestCase{TEXT("abc"), TEXT("bcd"), TEXT("bc")});
		Tests.Add(FTestCase{TEXT("ABC"), TEXT("BCD"), TEXT("BC")});
		Tests.Add(FTestCase{TEXT("012345"), TEXT("6789"), TEXT("")});
		Tests.Add(FTestCase{TEXT(""), TEXT(""), TEXT("")});
		Tests.Add(FTestCase{TEXT("abc"), TEXT(""), TEXT("")});
		Tests.Add(FTestCase{TEXT(""), TEXT("abc"), TEXT("")});

		for (const auto& Test : Tests)
		{
			const FString Actual = UGamedevHelperStringLibrary::Intersection(Test.CharsetA, Test.CharsetB);
			if (Actual != Test.Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected intersection of %s and %s to be '%s' got '%s'"), *Test.CharsetA, *Test.CharsetB, *Test.Expected, *Actual);
				return false;
			}
		}
		
		return true;	
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);
	
	return true;
}

bool FStringLibraryUnionTest::RunTest(const FString& Parameters)
{
	// https://en.wikipedia.org/wiki/Venn_diagram - Union
	
	struct FTestCase
	{
		FString CharsetA;
		FString CharsetB;
		FString Expected;
	};
	
	const auto Scenario = []()
	{
		TArray<FTestCase> Tests;
		Tests.Add(FTestCase{TEXT("abc"), TEXT("bcd"), TEXT("abcd")});
		Tests.Add(FTestCase{TEXT("ABC"), TEXT("BCD"), TEXT("ABCD")});
		Tests.Add(FTestCase{TEXT("012345"), TEXT("6789"), TEXT("0123456789")});
		Tests.Add(FTestCase{TEXT(""), TEXT(""), TEXT("")});
		Tests.Add(FTestCase{TEXT("abc"), TEXT(""), TEXT("abc")});
		Tests.Add(FTestCase{TEXT(""), TEXT("abc"), TEXT("abc")});

		for (const auto& Test : Tests)
		{
			const FString Actual = UGamedevHelperStringLibrary::Union(Test.CharsetA, Test.CharsetB);
			if (Actual != Test.Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected union of %s and %s to be '%s' got '%s'"), *Test.CharsetA, *Test.CharsetB, *Test.Expected, *Actual);
				return false;
			}
		}
		
		return true;	
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);
	
	return true;
}

bool FStringLibrarySymmetricDifferenceTest::RunTest(const FString& Parameters)
{
	// https://en.wikipedia.org/wiki/Venn_diagram - Symmetric Difference
	
	struct FTestCase
	{
		FString CharsetA;
		FString CharsetB;
		FString Expected;
	};
	
	const auto Scenario = []()
	{
		TArray<FTestCase> Tests;
		Tests.Add(FTestCase{TEXT("abc"), TEXT("bcd"), TEXT("ad")});
		Tests.Add(FTestCase{TEXT("ABC"), TEXT("BCD"), TEXT("AD")});
		Tests.Add(FTestCase{TEXT("012345"), TEXT("6789"), TEXT("0123456789")});
		Tests.Add(FTestCase{TEXT(""), TEXT(""), TEXT("")});
		Tests.Add(FTestCase{TEXT("abc"), TEXT(""), TEXT("abc")});
		Tests.Add(FTestCase{TEXT(""), TEXT("abc"), TEXT("abc")});

		for (const auto& Test : Tests)
		{
			const FString Actual = UGamedevHelperStringLibrary::SymmetricDifference(Test.CharsetA, Test.CharsetB);
			if (Actual != Test.Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected symmetric difference of %s and %s to be '%s' got '%s'"), *Test.CharsetA, *Test.CharsetB, *Test.Expected, *Actual);
				return false;
			}
		}
		
		return true;	
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);
	
	return true;
}

bool FStringLibraryDifferenceTest::RunTest(const FString& Parameters)
{
	// https://en.wikipedia.org/wiki/Complement_(set_theory) - Difference (Relative Complement)
	// SET A = {1,2,3}
	// SET B = {2,3,4}
	// Difference is the set of all elements that are members of A, but not members of B so => {1}
	
	struct FTestCase
	{
		FString CharsetA;
		FString CharsetB;
		FString Expected;
	};
	
	const auto Scenario = []()
	{
		TArray<FTestCase> Tests;
		Tests.Add(FTestCase{TEXT("abc"), TEXT("bcd"), TEXT("a")});
		Tests.Add(FTestCase{TEXT("ABC"), TEXT("BCD"), TEXT("A")});
		Tests.Add(FTestCase{TEXT("012345"), TEXT("6789"), TEXT("012345")});
		Tests.Add(FTestCase{TEXT("123"), TEXT("146"), TEXT("23")});
		Tests.Add(FTestCase{TEXT(""), TEXT(""), TEXT("")});
		Tests.Add(FTestCase{TEXT("abc"), TEXT(""), TEXT("abc")});
		Tests.Add(FTestCase{TEXT(""), TEXT("abc"), TEXT("")});
		Tests.Add(FTestCase{TEXT("abc"), TEXT("abc"), TEXT("")});

		for (const auto& Test : Tests)
		{
			const FString Actual = UGamedevHelperStringLibrary::Difference(Test.CharsetA, Test.CharsetB);
			if (Actual != Test.Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected difference of %s and %s to be '%s' got '%s'"), *Test.CharsetA, *Test.CharsetB, *Test.Expected, *Actual);
				return false;
			}
		}
		
		return true;	
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);
	
	return true;
}

bool FStringLibraryIsSubSetTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString CharsetA;
		FString CharsetB;
		bool Expected;
	};
	
	const auto Scenario = []()
	{
		TArray<FTestCase> Tests;
		Tests.Add(FTestCase{TEXT("abc"), TEXT("abcdef"), true});
		Tests.Add(FTestCase{TEXT("ABC"), TEXT("ABCDEF"), true});
		Tests.Add(FTestCase{TEXT("abc"), TEXT("feagbhc"), true});
		Tests.Add(FTestCase{TEXT("123"), TEXT("0569"), false});
		Tests.Add(FTestCase{TEXT(""), TEXT(""), true});
		Tests.Add(FTestCase{TEXT("abc"), TEXT(""), true});
		Tests.Add(FTestCase{TEXT(""), TEXT("abc"), true});
		Tests.Add(FTestCase{TEXT("abc"), TEXT("abc"), true});
		Tests.Add(FTestCase{TEXT("r0_wwWhC_P"), GamedevHelperConstants::AlphaMixed + GamedevHelperConstants::Digits + TEXT("_"), true});

		for (const auto& Test : Tests)
		{
			const bool Actual = UGamedevHelperStringLibrary::IsSubSet(Test.CharsetA, Test.CharsetB);
			if (Actual != Test.Expected)
			{
				UE_LOG(LogGamedevHelper, Error, TEXT("Expected %s to be subset of %s, got false"), *Test.CharsetA, *Test.CharsetB);
				return false;
			}
		}
		
		return true;	
	};

	FGamedevHelperTester::ExpectTestScenarioPass(Scenario);
	
	return true;
}

bool FStringLibraryRegexReplaceTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString Input;
		FString Expected;
	};
	TArray<FTestCase> Tests;
	Tests.Add(FTestCase{TEXT("_aaa_"), TEXT("_aaa_")});
	Tests.Add(FTestCase{TEXT("r0};wwWhC%P,%aaa,%"), TEXT("r0_wwWhC_P_aaa_")});

	for (const auto& Test : Tests)
	{
		const FString Actual = UGamedevHelperStringLibrary::RegexReplace(Test.Input, TEXT("[^0-9a-zA-Z]+"), TEXT("_"));
		if (Actual != Test.Expected)
		{
			UE_LOG(LogGamedevHelper, Error, TEXT("Expected '%s', got '%s', for input '%s'"), *Test.Expected, *Actual, *Test.Input);
			return false;
		}
	}
	
	
	return true;
}




// todo:ashe23 add 100% test coverage
