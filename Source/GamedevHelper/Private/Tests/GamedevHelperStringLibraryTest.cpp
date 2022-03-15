// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GamedevHelperStringLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FStringLibraryGetFirstLetterTest,
	"Plugins.GamedevHelper.Utility.String.GetFirstLetter",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FStringLibraryGetFirstLetterTest::RunTest(const FString& Parameters)
{
	return true;
}
