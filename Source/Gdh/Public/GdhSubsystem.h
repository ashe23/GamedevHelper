// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GdhSubsystem.generated.h"

class UMoviePipelineSetting;
class UMoviePipelineMasterConfig;

UCLASS(DisplayName="GamedevHelper Subsystem")
class UGdhSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// rendering library
	static bool IsValidJobSetting(UMoviePipelineSetting* Setting);
	static bool IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig);
	static FString GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig);
};