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
