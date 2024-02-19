// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibEditor.h"
#include "GdhConstants.h"
// Engine Headers
#include "IContentBrowserSingleton.h"
#include "Editor.h"
#include "ShaderCompiler.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Subsystems/AssetEditorSubsystem.h"

void UGdhLibEditor::TryOpenFile(const FString& Path)
{
	if (Path.IsEmpty()) return;
	if (!FPaths::FileExists(Path)) return;

	FPlatformProcess::LaunchFileInDefaultExternalApplication(*Path);
}

void UGdhLibEditor::OpenAssetEditor(const FAssetData& Asset)
{
	if (!Asset.IsValid()) return;
	if (!GEditor) return;

	TArray<FName> AssetNames;
	AssetNames.Add(Asset.ToSoftObjectPath().GetAssetPathName());

	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorsForAssets(AssetNames);
}

void UGdhLibEditor::OpenAssetInContentBrowser(const FAssetData& Asset, const bool bSpawnNewBrowser)
{
	if (!Asset.IsValid()) return;

	GetModuleContentBrowser().Get().SyncBrowserToAssets(TArray<FAssetData>{Asset}, false, true, FName{}, bSpawnNewBrowser);
}

void UGdhLibEditor::OpenPathInFileExplorer(const FString& Path)
{
	if (Path.IsEmpty()) return;

	FPlatformProcess::ExploreFolder(*Path);
}

void UGdhLibEditor::CloseAllEditors()
{
	if (!GEditor) return;

	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseAllAssetEditors();
}

bool UGdhLibEditor::EditorInPlayMode()
{
	return GEditor && GEditor->PlayWorld || GIsPlayInEditorWorld;
}

void UGdhLibEditor::ShaderCompilationEnable()
{
	if (!GShaderCompilingManager) return;

	GShaderCompilingManager->SkipShaderCompilation(false);
}

void UGdhLibEditor::ShaderCompilationDisable()
{
	if (!GShaderCompilingManager) return;

	GShaderCompilingManager->SkipShaderCompilation(true);
}

void UGdhLibEditor::ShowNotification(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.Text = FText::FromString(Msg);
	Info.ExpireDuration = Duration;

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(State);
}

void UGdhLibEditor::ShowNotificationWithOutputLog(const FString& Msg, const SNotificationItem::ECompletionState State, const float Duration)
{
	FNotificationInfo Info{FText::FromString(Msg)};
	Info.Text = FText::FromString(Msg);
	Info.ExpireDuration = Duration;
	Info.Hyperlink = FSimpleDelegate::CreateLambda([]()
	{
		FGlobalTabmanager::Get()->TryInvokeTab(FName{TEXT("OutputLog")});
	});
	Info.HyperlinkText = FText::FromString(TEXT("Show OutputLog..."));

	const auto NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);
	if (!NotificationPtr.IsValid()) return;

	NotificationPtr.Get()->SetCompletionState(State);
}

FAssetToolsModule& UGdhLibEditor::GetModuleAssetTools()
{
	return FModuleManager::LoadModuleChecked<FAssetToolsModule>(GdhConstants::ModuleAssetTools);
}

FAssetRegistryModule& UGdhLibEditor::GetModuleAssetRegistry()
{
	return FModuleManager::LoadModuleChecked<FAssetRegistryModule>(GdhConstants::ModuleAssetRegistry);
}

FContentBrowserModule& UGdhLibEditor::GetModuleContentBrowser()
{
	return FModuleManager::LoadModuleChecked<FContentBrowserModule>(GdhConstants::ModuleContentBrowser);
}

FPropertyEditorModule& UGdhLibEditor::GetModulePropertyEditor()
{
	return FModuleManager::LoadModuleChecked<FPropertyEditorModule>(GdhConstants::ModulePropertyEditor);
}
