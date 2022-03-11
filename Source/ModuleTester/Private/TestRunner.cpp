// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "TestRunner.h"
#include "ModuleTester.h"

void FTestRunner::ExpectTrue(const bool Expression, const FString& Input, const FString& TestCaseName)
{
	FTestCase TestCase;
	TestCase.Name = TestCaseName;
	TestCase.bExpected = true;
	TestCase.bActual = Expression;
	if (!Input.IsEmpty())
	{
		TestCase.Input = Input;
	}

	Tests.Add(TestCase);
}

void FTestRunner::ExpectFalse(const bool Expression, const FString& Input, const FString& TestCaseName)
{
	FTestCase TestCase;
	TestCase.Name = TestCaseName;
	TestCase.bExpected = false;
	TestCase.bActual = Expression;
	if (!Input.IsEmpty())
	{
		TestCase.Input = Input;
	}

	Tests.Add(TestCase);
}

bool FTestRunner::GetResult()
{
	for (int32 TestId = 0; TestId < Tests.Num(); ++TestId)
	{
		if (Tests[TestId].bExpected != Tests[TestId].bActual)
		{
			UE_LOG(LogModuleTester, Error, TEXT("TestCase [%s][%d] - failed"), *Tests[TestId].Name, TestId);
			UE_LOG(LogModuleTester, Error, TEXT("Expected %s Got %s"), Tests[TestId].bExpected ? TEXT("True") : TEXT("False"), Tests[TestId].bActual ? TEXT("True") : TEXT("False"));
			UE_LOG(LogModuleTester, Error, TEXT("Input    : %s"), Tests[TestId].Input.IsEmpty() ? TEXT("Empty String") : *Tests[TestId].Input);
			
			if (!Tests[TestId].Expected.IsEmpty())
			{
				UE_LOG(LogModuleTester, Error, TEXT("Expected : %s"), Tests[TestId].Expected.IsEmpty() ? TEXT("Empty String") : *Tests[TestId].Expected);
			}
			if (!Tests[TestId].Actual.IsEmpty())
			{
				UE_LOG(LogModuleTester, Error, TEXT("Actual   : %s"), Tests[TestId].Actual.IsEmpty() ? TEXT("Empty String") : *Tests[TestId].Actual);
			}
			return false;
		}
	}

	return true;
}
