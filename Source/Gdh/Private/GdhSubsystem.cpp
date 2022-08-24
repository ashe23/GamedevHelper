// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhSubsystem.h"
// Engine Headers
#include "MoviePipelineMasterConfig.h"

void UGdhSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGdhSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UGdhSubsystem::IsValidJobSetting(UMoviePipelineSetting* Setting)
{
	if (!Setting) return false;

	Setting->ValidateState();

	return Setting->GetValidationState() == EMoviePipelineValidationState::Valid;
}

bool UGdhSubsystem::IsValidMasterConfig(const UMoviePipelineMasterConfig* MasterConfig)
{
	if (!MasterConfig) return false;
	
	for (const auto& Setting : MasterConfig->GetAllSettings())
	{
		if (!IsValidJobSetting(Setting))
		{
			return false;
		}
	}

	return true;
}

FString UGdhSubsystem::GetMasterConfigValidationMsg(const UMoviePipelineMasterConfig* MasterConfig)
{
	if (!MasterConfig) return TEXT("");
	
	for (const auto& Setting : MasterConfig->GetAllSettings())
	{
		Setting->ValidateState();

		if (Setting->GetValidationState() == EMoviePipelineValidationState::Errors || Setting->GetValidationState() == EMoviePipelineValidationState::Warnings)
		{
			const TArray<FText> ValidationResults = Setting->GetValidationResults();

			FString ValidateErrorText;

			for (const auto& ValidationResult : ValidationResults)
			{
				ValidateErrorText.Append(ValidationResult.ToString() + TEXT("\n"));
			}

			return ValidateErrorText;
		}
	}

	return TEXT("");
}
