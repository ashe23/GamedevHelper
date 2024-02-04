// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhPluginSettings.generated.h"

class UGdhNamingPolicyAsset;

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="GamedevHelper Plugin Settings")
class UGdhPluginSettings final : public UDeveloperSettings
{
public:
	GENERATED_BODY()

	UGdhPluginSettings();
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="Naming Policy")
	TSoftObjectPtr<UGdhNamingPolicyAsset> DefaultNamingPolicyAsset;
};
