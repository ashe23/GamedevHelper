// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerQueueSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Queue Settings")
class UGamedevHelperRenderingManagerQueueSettings : public UObject
{
	GENERATED_BODY()
public:
	UGamedevHelperRenderingManagerQueueSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere)
	TArray<FGamedevHelperRenderElement> RenderElements;
};