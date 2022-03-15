// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GamedevHelperTester.h"

template <typename F>
bool FGamedevHelperTester::ExpectTestScenarioPass(F TestScenario)
{
	return TestScenario();
}

bool FGamedevHelperTester::ExpectTrue(const bool bExpression, const FString& FailMsg)
{
	if (bExpression == false)
	{
		UE_LOG(LogGamedevHelper, Error, TEXT("%s"), *FailMsg);
		return false;
	}

	return true;
}

bool FGamedevHelperTester::ExpectStrEmpty(const FString& InputString)
{
	if (InputString.IsEmpty() == false)
	{
		UE_LOG(LogGamedevHelper, Error, TEXT("Expected empty string"));
		UE_LOG(LogGamedevHelper, Error, TEXT("Actual: %s"), *InputString);
		return false;
	}

	return true;
}

bool FGamedevHelperTester::ExpectStrNotEmpty(const FString& InputString)
{
	if (InputString.IsEmpty())
	{
		UE_LOG(LogGamedevHelper, Error, TEXT("Expected not empty string"));
		UE_LOG(LogGamedevHelper, Error, TEXT("Actual: empty string"));
		return false;
	}

	return true;
}

bool FGamedevHelperTester::ExpectStrEqual(const FString& Input, const FString& Expected)
{
	if (Input.Equals(Expected, ESearchCase::CaseSensitive) == false)
	{
		UE_LOG(LogGamedevHelper, Error, TEXT("Expected %s to equal %s, got false"), *Input, *Expected);
		return false;
	}

	return true;
}
