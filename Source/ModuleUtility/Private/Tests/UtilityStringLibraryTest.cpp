// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UtilityStringLibrary.h"
#include "ModuleUtility.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetFirstLetterTest,
	"Plugins.GamedevHelper.Utility.String.GetFirstLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetLastLetterTest,
	"Plugins.GamedevHelper.Utility.String.GetLastLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryRemoveFirstLetterTest,
	"Plugins.GamedevHelper.Utility.String.RemoveFirstLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryRemoveLastLetterTest,
	"Plugins.GamedevHelper.Utility.String.RemoveLastLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryIntersectionTest,
	"Plugins.GamedevHelper.Utility.String.Intersection",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryUnionTest,
	"Plugins.GamedevHelper.Utility.String.Union",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryDifferenceTest,
	"Plugins.GamedevHelper.Utility.String.Difference",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibrarySymmetricDifferenceTest,
	"Plugins.GamedevHelper.Utility.String.SymmetricDifference",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryIsSubSetTest,
	"Plugins.GamedevHelper.Utility.String.IsSubSet",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryContainsTest,
	"Plugins.GamedevHelper.Utility.String.Contains",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryContainsOnlyTest,
	"Plugins.GamedevHelper.Utility.String.ContainsOnly",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryContainsLettersTest,
	"Plugins.GamedevHelper.Utility.String.ContainsLetters",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryContainsDigitsTest,
	"Plugins.GamedevHelper.Utility.String.ContainsDigits",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryContainsOnlyLettersTest,
	"Plugins.GamedevHelper.Utility.String.ContainsOnlyLetters",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryContainsOnlyDigitsTest,
	"Plugins.GamedevHelper.Utility.String.ContainsOnlyDigits",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

static const FString EmptyString = TEXT("");

bool FStringLibraryGetFirstLetterTest::RunTest(const FString& Parameters)
{
	// if string is empty , must return original string
	const FString Actual = UGamedevHelperStringLibrary::GetFirstLetter(EmptyString);
	if (Actual.IsEmpty() == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected empty string, got %s"), *Actual);
		return false;
	}

	// length of returned string must always be 1 (if not empty)
	const FString FirstLetter = UGamedevHelperStringLibrary::GetFirstLetter(TEXT("abcdef"));
	if (FirstLetter.Len() != 1)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected string length to be 1, got %d"), FirstLetter.Len());
		return false;
	}

	if (!FirstLetter.Equals(TEXT("a"), ESearchCase::CaseSensitive))
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected first letter to be 'a', got %s"), *FirstLetter);
		return false;
	}


	return true;
}

bool FStringLibraryGetLastLetterTest::RunTest(const FString& Parameters)
{
	const FString Actual = UGamedevHelperStringLibrary::GetLastLetter(EmptyString);

	if (Actual.IsEmpty() == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected empty string, got %s"), *Actual);
		return false;
	}

	const FString SomeString = TEXT("abcdef");
	const FString LastLetter = UGamedevHelperStringLibrary::GetLastLetter(SomeString);

	if (LastLetter.Len() != 1)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected string length to be 1, got %d"), LastLetter.Len());
		return false;
	}

	if (LastLetter.Equals(TEXT("f")) == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected last letter to be 'f', got %s"), *LastLetter);
		return false;
	}

	return true;
}

bool FStringLibraryRemoveFirstLetterTest::RunTest(const FString& Parameters)
{
	const FString Actual = UGamedevHelperStringLibrary::RemoveFirstLetter(EmptyString);
	if (Actual.IsEmpty() == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected empty string, got %s"), *Actual);
		return false;
	}

	const FString Input = TEXT("abcdef");
	const FString Expected = TEXT("bcdef");
	if (UGamedevHelperStringLibrary::RemoveFirstLetter(Input).Equals(Expected, ESearchCase::CaseSensitive) == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected %s, got %s"), *Expected, *UGamedevHelperStringLibrary::RemoveFirstLetter(Input));
		return false;
	}

	return true;
}

bool FStringLibraryRemoveLastLetterTest::RunTest(const FString& Parameters)
{
	const FString Actual = UGamedevHelperStringLibrary::RemoveLastLetter(EmptyString);
	if (Actual.IsEmpty() == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected empty string, got %s"), *Actual);
		return false;
	}

	const FString Input = TEXT("abcdef");
	const FString Expected = TEXT("abcde");
	if (UGamedevHelperStringLibrary::RemoveLastLetter(Input).Equals(Expected, ESearchCase::CaseSensitive) == false)
	{
		UE_LOG(LogModuleUtility, Error, TEXT("Expected %s, got %s"), *Expected, *UGamedevHelperStringLibrary::RemoveLastLetter(Input));
		return false;
	}

	return true;
}

bool FStringLibraryIntersectionTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		FString Expected;
		FString Actual;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const FString& Expected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = Expected;
			TestCase.Actual = UGamedevHelperStringLibrary::Intersection(StringA, StringB);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected.Equals(Test.Actual) == false)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected intersection of %s and %s is %s, got %s"), *Test.StringA, *Test.StringB, *Test.Expected, *Test.Actual);
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abcdef"), TEXT("abc"));
	Runner.AddTest(TEXT("ABC"), TEXT("abcdef"), TEXT(""));
	Runner.AddTest(TEXT("ABC"), TEXT("ABCdef"), TEXT("ABC"));
	Runner.AddTest(TEXT("aBc"), TEXT("aBcdef"), TEXT("aBc"));
	Runner.AddTest(TEXT(""), TEXT("abcdef"), TEXT(""));
	Runner.AddTest(TEXT("abc"), TEXT(""), TEXT(""));
	Runner.AddTest(TEXT("012"), TEXT("982563"), TEXT("2"));

	return Runner.GetResult();
}

bool FStringLibraryUnionTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		FString Expected;
		FString Actual;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const FString& Expected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = Expected;
			TestCase.Actual = UGamedevHelperStringLibrary::Union(StringA, StringB);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected.Equals(Test.Actual) == false)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected union of %s and %s is %s, got %s"), *Test.StringA, *Test.StringB, *Test.Expected, *Test.Actual);
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abcdef"), TEXT("abcdef"));
	Runner.AddTest(TEXT("ABC"), TEXT("abcdef"), TEXT("ABCabcdef"));
	Runner.AddTest(TEXT("ABC"), TEXT("ABCdef"), TEXT("ABCdef"));
	Runner.AddTest(TEXT("aBc"), TEXT("aBcdef"), TEXT("aBcdef"));
	Runner.AddTest(TEXT(""), TEXT("abcdef"), TEXT("abcdef"));
	Runner.AddTest(TEXT("abc"), TEXT(""), TEXT("abc"));
	Runner.AddTest(TEXT("012"), TEXT("982563"), TEXT("01298563"));

	return Runner.GetResult();
}

bool FStringLibraryDifferenceTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		FString Expected;
		FString Actual;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const FString& Expected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = Expected;
			TestCase.Actual = UGamedevHelperStringLibrary::Difference(StringA, StringB);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected.Equals(Test.Actual) == false)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected difference of %s and %s is %s, got %s"), *Test.StringA, *Test.StringB, *Test.Expected, *Test.Actual);
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abd"), TEXT("c"));
	Runner.AddTest(TEXT("ABC"), TEXT("abd"), TEXT("ABC"));
	Runner.AddTest(TEXT(""), TEXT("abc"), TEXT(""));
	Runner.AddTest(TEXT("ABC"), TEXT(""), TEXT("ABC"));
	Runner.AddTest(TEXT(""), TEXT(""), TEXT(""));
	Runner.AddTest(TEXT("123"), TEXT("0123456789"), TEXT(""));
	Runner.AddTest(TEXT("123"), TEXT("013456789"), TEXT("2"));

	return Runner.GetResult();
}

bool FStringLibrarySymmetricDifferenceTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		FString Expected;
		FString Actual;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const FString& Expected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = Expected;
			TestCase.Actual = UGamedevHelperStringLibrary::SymmetricDifference(StringA, StringB);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected.Equals(Test.Actual) == false)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected symmetric difference of %s and %s is %s, got %s"), *Test.StringA, *Test.StringB, *Test.Expected, *Test.Actual);
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abcdef"), TEXT("def"));
	Runner.AddTest(TEXT("ABC"), TEXT("abcdef"), TEXT("ABCabcdef"));
	Runner.AddTest(TEXT(""), TEXT("abc"), TEXT("abc"));
	Runner.AddTest(TEXT("ABC"), TEXT(""), TEXT("ABC"));
	Runner.AddTest(TEXT(""), TEXT(""), TEXT(""));
	Runner.AddTest(TEXT("123"), TEXT("0123456789"), TEXT("0456789"));

	return Runner.GetResult();
}

bool FStringLibraryIsSubSetTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::IsSubSet(StringA, StringB);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to be subset of %s, got %s"), *Test.StringA, *Test.StringB, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abcdef"), true);
	Runner.AddTest(TEXT("abc"), TEXT("defgh"), false);
	Runner.AddTest(TEXT("abc"), TEXT("ABC"), false);
	Runner.AddTest(TEXT("012"), TEXT("12356089"), true);

	return Runner.GetResult();
}

bool FStringLibraryContainsTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const ESearchCase::Type SearchCase, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::Contains(StringA, StringB, SearchCase);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to contain character set of %s, got %s"), *Test.StringA, *Test.StringB, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abcdef"), ESearchCase::CaseSensitive, true);
	Runner.AddTest(TEXT("abc"), TEXT("ABC"), ESearchCase::CaseSensitive, false);
	Runner.AddTest(TEXT("abc"), TEXT("ABC"), ESearchCase::IgnoreCase, true);
	Runner.AddTest(TEXT(""), TEXT(""), ESearchCase::CaseSensitive, false);
	Runner.AddTest(TEXT(""), TEXT(""), ESearchCase::IgnoreCase, false);
	Runner.AddTest(TEXT("12"), TEXT("5671290"), ESearchCase::IgnoreCase, true);
	Runner.AddTest(TEXT("__--"), TEXT("_-"), ESearchCase::IgnoreCase, true);

	return Runner.GetResult();
}

bool FStringLibraryContainsOnlyTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		FString StringB;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const FString& StringB, const ESearchCase::Type SearchCase, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.StringB = StringB;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::ContainsOnly(StringA, StringB, SearchCase);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to contain only character set of %s, got %s"), *Test.StringA, *Test.StringB, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), TEXT("abcdef"), ESearchCase::CaseSensitive, true);
	Runner.AddTest(TEXT("abc"), TEXT("ABC"), ESearchCase::CaseSensitive, false);
	Runner.AddTest(TEXT("abc"), TEXT("ABC"), ESearchCase::IgnoreCase, true);
	Runner.AddTest(TEXT(""), TEXT(""), ESearchCase::CaseSensitive, true);
	Runner.AddTest(TEXT(""), TEXT(""), ESearchCase::IgnoreCase, true);
	Runner.AddTest(TEXT("12"), TEXT("5671290"), ESearchCase::IgnoreCase, true);
	Runner.AddTest(TEXT("__--"), TEXT("_-"), ESearchCase::IgnoreCase, true);
	Runner.AddTest(TEXT("abc"), TEXT("def"), ESearchCase::IgnoreCase, false);
	Runner.AddTest(TEXT("abc"), TEXT("DEF"), ESearchCase::CaseSensitive, false);
	Runner.AddTest(TEXT("abc"), TEXT("afc"), ESearchCase::CaseSensitive, false);

	return Runner.GetResult();
}

bool FStringLibraryContainsLettersTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::ContainsLetters(StringA);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to contain letters, got %s"), *Test.StringA, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), true);
	Runner.AddTest(TEXT("ABC"), true);
	Runner.AddTest(TEXT("abc12"), true);
	Runner.AddTest(TEXT("ABC12"), true);
	Runner.AddTest(TEXT("_-"), false);
	Runner.AddTest(TEXT("_-01ab"), true);
	Runner.AddTest(TEXT("123456789"), false);

	return Runner.GetResult();
}

bool FStringLibraryContainsDigitsTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::ContainsDigits(StringA);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to contain digits, got %s"), *Test.StringA, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), false);
	Runner.AddTest(TEXT("ABC"), false);
	Runner.AddTest(TEXT("abc12"), true);
	Runner.AddTest(TEXT("ABC12"), true);
	Runner.AddTest(TEXT("_-"), false);
	Runner.AddTest(TEXT("_-01ab"), true);
	Runner.AddTest(TEXT("123456789"), true);

	return Runner.GetResult();
}

bool FStringLibraryContainsOnlyDigitsTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::ContainsOnlyDigits(StringA);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to contain only digits, got %s"), *Test.StringA, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), false);
	Runner.AddTest(TEXT("ABC"), false);
	Runner.AddTest(TEXT("abc12"), false);
	Runner.AddTest(TEXT("ABC12"), false);
	Runner.AddTest(TEXT("_-"), false);
	Runner.AddTest(TEXT("_-01ab"), false);
	Runner.AddTest(TEXT("123456789"), true);

	return Runner.GetResult();
}

bool FStringLibraryContainsOnlyLettersTest::RunTest(const FString& Parameters)
{
	struct FTestCase
	{
		FString StringA;
		bool Actual;
		bool Expected;
	};

	struct FTestRunner
	{
		void AddTest(const FString& StringA, const bool bExpected)
		{
			FTestCase TestCase;
			TestCase.StringA = StringA;
			TestCase.Expected = bExpected;
			TestCase.Actual = UGamedevHelperStringLibrary::ContainsOnlyLetters(StringA);
			Tests.Add(TestCase);
		}

		bool GetResult()
		{
			for (const auto& Test : Tests)
			{
				if (Test.Expected != Test.Actual)
				{
					UE_LOG(LogModuleUtility, Error, TEXT("Expected %s to contain only digits, got %s"), *Test.StringA, Test.Actual ? TEXT("True") : TEXT("False"));
					return false;
				}
			}

			return true;
		}

	private:
		TArray<FTestCase> Tests;
	};

	FTestRunner Runner;
	Runner.AddTest(TEXT("abc"), true);
	Runner.AddTest(TEXT("ABC"), true);
	Runner.AddTest(TEXT("abc12"), false);
	Runner.AddTest(TEXT("ABC12"), false);
	Runner.AddTest(TEXT("_-"), false);
	Runner.AddTest(TEXT("_-01ab"), false);
	Runner.AddTest(TEXT("123456789"), false);

	return Runner.GetResult();
}
