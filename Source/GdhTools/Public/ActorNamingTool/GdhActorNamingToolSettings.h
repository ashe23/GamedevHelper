// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhEnums.h"
#include "GdhStructs.h"
#include "GdhActorNamingToolSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Settings")
class UGdhActorNamingToolSettings : public UObject
{
public:
	GENERATED_BODY()

	UGdhActorNamingToolSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category="NamingConvention", DisplayName="Asset Affix Mappings", meta=(RequiredAssetDataTags="RowStructure=GdhActorAffixRow"))
	TSoftObjectPtr<UDataTable> Mappings;
};
