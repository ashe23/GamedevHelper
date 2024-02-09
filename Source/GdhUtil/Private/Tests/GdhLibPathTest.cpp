// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "GdhLibPath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhLibPath, "Gdh.Library.Path.Repeat", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FGdhLibPath::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Empty String"), UGdhLibPath::Normalize(TEXT("")), TEXT(""));

	return true;
}


#endif
