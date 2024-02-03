#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGdhNamingPolicyTest, "Gdh.NamingPolicy.SomeFunc", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGdhNamingPolicyTest::RunTest(const FString& Parameters)
{
	// Make the test pass by returning true, or fail by returning false.
	return true;
}
