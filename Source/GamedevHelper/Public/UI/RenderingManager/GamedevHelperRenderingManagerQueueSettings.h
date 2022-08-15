// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamedevHelperTypes.h"
#include "GamedevHelperRenderingManagerQueueSettings.generated.h"

DECLARE_DELEGATE(FQueueSettingsDelegate);

UCLASS(Config=EditorPerProjectUserSettings, DisplayName="Queue Settings")
class UGamedevHelperRenderingManagerQueueSettings : public UObject
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category="QueueSettings")
	FString GetErrorMsg() const;

	UFUNCTION(BlueprintCallable, Category="QueueSettings")
	void Validate();
	
	UFUNCTION(BlueprintCallable, Category="QueueSettings")
	bool IsValid() const;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category="QueueSettings", meta=(AllowedClasses="MoviePipelineQueue"))
	TArray<FSoftObjectPath> QueueAssets;

	FQueueSettingsDelegate QueueSettingsDelegate;

private:
	FString ErrorMsg;
};
