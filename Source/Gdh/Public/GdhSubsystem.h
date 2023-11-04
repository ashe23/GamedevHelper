// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "GdhSubsystem.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="GamedevHelperSubsystem")
class UGdhSubsystem final : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/**
	 * @brief Returns all assets in project (particularly Content folder)
	 * @param Assets TArray<FAssetData>
	 */
	UFUNCTION(BlueprintCallable, Category="GamedevHelperSubsystem|Lib_Asset")
	static void GetAssetsAll(TArray<FAssetData>& Assets);

	static FAssetRegistryModule& GetModuleAssetRegistry();
};
