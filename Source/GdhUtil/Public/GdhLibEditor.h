// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GdhLibEditor.generated.h"

class FAssetRegistryModule;
class FAssetToolsModule;

UCLASS(meta=(BlueprintThreadSafe, ScriptName="GdhEditorLibrary"))
class GDHUTIL_API UGdhLibEditor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void TryOpenFile(const FString& Path);
	static void OpenAssetEditor(const FAssetData& Asset);
	static void OpenAssetInContentBrowser(const FAssetData& Asset, const bool bSpawnNewBrowser);
	static void OpenPathInFileExplorer(const FString& Path);
	static void ShowNotification(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration);
	static void ShowNotificationWithOutputLog(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration);
	static void CloseAllEditors();
	static bool EditorInPlayMode();
	static void ShaderCompilationEnable();
	static void ShaderCompilationDisable();

	static FAssetToolsModule& GetModuleAssetTools();
	static FAssetRegistryModule& GetModuleAssetRegistry();
	static FContentBrowserModule& GetModuleContentBrowser();
	static FPropertyEditorModule& GetModulePropertyEditor();
};
