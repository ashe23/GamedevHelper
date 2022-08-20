// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhRenderingManager.h"
// Engine Headers
#include "IPythonScriptPlugin.h"

void FGdhRenderingManager::StartupModule()
{
	
}

void FGdhRenderingManager::ShutdownModule()
{
	
}

bool FGdhRenderingManager::SupportsDynamicReloading()
{
	return false;
}

bool FGdhRenderingManager::IsGameModule() const
{
	return false;
}

FString FGdhRenderingManager::TryFindFFmpegExePath()
{
	FPythonCommandEx CommandEx;
	CommandEx.Command = TEXT("ffmpeg_location.py");
	CommandEx.ExecutionMode = EPythonCommandExecutionMode::ExecuteFile;
	
	if (!IPythonScriptPlugin::Get()->ExecPythonCommandEx(CommandEx))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to run %s py script"), *CommandEx.Command);
		return TEXT("");
	}

	if (CommandEx.LogOutput.Num() == 0)
	{
		return TEXT("");
	}

	if (const auto [Type, Output] = CommandEx.LogOutput[0]; Type == EPythonLogOutputType::Info)
	{
		if (IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile(); PlatformFile.FileExists(*Output))
		{
			return Output;
		}
	}

	return TEXT("");
}

IMPLEMENT_MODULE(FGdhRenderingManager, GdhRenderingManager);
