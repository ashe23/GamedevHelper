// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FTestCase
{
	FString Name;
	FString Input;
	FString Expected;
	FString Actual;
	bool bExpected;
	bool bActual;
};

struct FTestRunner
{
	void ExpectTrue(const bool Expression, const FString& Input, const FString& TestCaseName);
	void ExpectFalse(const bool Expression, const FString& Input, const FString& TestCaseName);
	bool GetResult();
private:
	TArray<FTestCase> Tests;
};
