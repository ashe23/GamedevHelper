// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "GdhLibEncoder.h"
#include "MoviePipelineMasterConfig.h"
#include "MoviePipelineOutputSetting.h"
#include "MoviePipelineImageSequenceOutput.h"

FString UGdhLibEncoder::GetImageExtension(const UMoviePipelineMasterConfig* Config) {
	if (!Config) return {};

	const auto SettingsAll = Config->GetAllSettings();

	for (const auto& Setting : SettingsAll) {
		if (Cast<UMoviePipelineImageSequenceOutput_PNG>(Setting)) return TEXT("png");
		if (Cast<UMoviePipelineImageSequenceOutput_JPG>(Setting)) return TEXT("jpg");
		if (Cast<UMoviePipelineImageSequenceOutput_BMP>(Setting)) return TEXT("bmp");
		// for exr, we cant use UMoviePipelineImageSequenceOutput_EXR class because its private to MoviePipelineRenderPasses module,
		// so to determine if its exr we're just casting to base class and if it casted successfully we assume its exr,
		// because until that all other image formats are already checked.
		// There maybe situations where user will activate multiple image format settings, but in that case only first one will be used.
		// So its up to user to be attentive and use correct MRQ render settings.
		if (Cast<UMoviePipelineImageSequenceOutputBase>(Setting)) return TEXT("exr");
	}

	return {};
}

int32 UGdhLibEncoder::GetZeroPadding(const UMoviePipelineMasterConfig* Config) {
	if (!Config) return 0;

	const UMoviePipelineOutputSetting* OutputSetting = Config->FindSetting<UMoviePipelineOutputSetting>();
	if (!OutputSetting) return 0;

	return OutputSetting->ZeroPadFrameNumbers;
}
