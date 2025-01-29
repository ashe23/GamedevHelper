// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhVideoEncoderToolSettings.generated.h"

class UGdhVideoPipeline;

UCLASS(Config = EditorPerProjectUserSettings, DisplayName = "Settings")
class UGdhVideoEncoderToolSettings : public UObject
{
  public:

	GENERATED_BODY()

	UGdhVideoEncoderToolSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "General")
	TSoftObjectPtr<UGdhVideoPipeline> Pipeline;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
