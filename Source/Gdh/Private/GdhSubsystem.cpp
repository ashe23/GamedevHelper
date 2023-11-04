// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSubsystem.h"
#include "Gdh.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

#if WITH_EDITOR
void UGdhSubsystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif

void UGdhSubsystem::GetAssetsAll(TArray<FAssetData>& Assets)
{
	if (GetModuleAssetRegistry().Get().IsLoadingAssets()) return;

	Assets.Reset();

	GetModuleAssetRegistry().Get().GetAssetsByPath(GdhConstants::PathRoot, Assets, true);
}

FAssetRegistryModule& UGdhSubsystem::GetModuleAssetRegistry()
{
	return FModuleManager::LoadModuleChecked<FAssetRegistryModule>(GdhConstants::ModuleAssetRegistry);
}

FPropertyEditorModule& UGdhSubsystem::GetModulePropertyEditor()
{
	return FModuleManager::LoadModuleChecked<FPropertyEditorModule>(GdhConstants::ModulePropertyEditor);
}
