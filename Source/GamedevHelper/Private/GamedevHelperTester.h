// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelper.h"

struct FGamedevHelperTester
{
	template<typename F>
	static bool ExpectTestScenarioPass(F TestScenario);
	
	static bool ExpectTrue(const bool bExpression, const FString& FailMsg);
	static bool ExpectStrEmpty(const FString& InputString);
	static bool ExpectStrNotEmpty(const FString& InputString);
	static bool ExpectStrEqual(const FString& Input, const FString& Expected);
};


