// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhMovieRenderSettings.h"
// Engine Headers
#include "MoviePipelineAntiAliasingSetting.h"
#include "MoviePipelineCameraSetting.h"
#include "MoviePipelineConsoleVariableSetting.h"
#include "MoviePipelineHighResSetting.h"
#include "MoviePipelineMasterConfig.h"
#include "MoviePipelineWidgetRenderSetting.h"

#if WITH_EDITOR
void UGdhMovieRenderSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
}
#endif

UMoviePipelineMasterConfig* UGdhMovieRenderSettings::CreateMasterConfig() const
{
	UMoviePipelineMasterConfig* MasterConfig = NewObject<UMoviePipelineMasterConfig>();
	if (!MasterConfig) return nullptr;

	if (bSettingsAAEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineAntiAliasingSetting> AntiAliasingSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineAntiAliasingSetting::StaticClass()))
		{
			AntiAliasingSetting->SpatialSampleCount = SpatialSampleCount;
			AntiAliasingSetting->TemporalSampleCount = TemporalSampleCount;
			AntiAliasingSetting->bOverrideAntiAliasing = bOverrideAntiAliasing;
			AntiAliasingSetting->AntiAliasingMethod = AntiAliasingMethod;
			AntiAliasingSetting->RenderWarmUpCount = RenderWarmUpCount;
			AntiAliasingSetting->bUseCameraCutForWarmUp = bUseCameraCutForWarmUp;
			AntiAliasingSetting->EngineWarmUpCount = EngineWarmUpCount;
			AntiAliasingSetting->bRenderWarmUpFrames = bRenderWarmUpFrames;
		}
	}

	if (bSettingsConsoleVariablesEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineConsoleVariableSetting> ConsoleVariableSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineConsoleVariableSetting::StaticClass()))
		{
			ConsoleVariableSetting->ConsoleVariables = ConsoleVariables;
			ConsoleVariableSetting->StartConsoleCommands = StartConsoleCommands;
			ConsoleVariableSetting->EndConsoleCommands = EndConsoleCommands;
		}
	}

	if (bSettingsGameOverrideEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineGameOverrideSetting> GameOverrideSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineGameOverrideSetting::StaticClass()))
		{
			GameOverrideSetting->GameModeOverride = GameModeOverride;
			GameOverrideSetting->bCinematicQualitySettings = bCinematicQualitySettings;
			GameOverrideSetting->TextureStreaming = TextureStreaming;
			GameOverrideSetting->bUseLODZero = bUseLODZero;
			GameOverrideSetting->bDisableHLODs = bDisableHLODs;
			GameOverrideSetting->bUseHighQualityShadows = bUseHighQualityShadows;
			GameOverrideSetting->ShadowDistanceScale = ShadowDistanceScale;
			GameOverrideSetting->ShadowRadiusThreshold = ShadowRadiusThreshold;
			GameOverrideSetting->bOverrideViewDistanceScale = bOverrideViewDistanceScale;
			GameOverrideSetting->ViewDistanceScale = ViewDistanceScale;
			GameOverrideSetting->bFlushGrassStreaming = bFlushGrassStreaming;
		}
	}

	if (bSettingsUIEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineWidgetRenderer> WidgetRenderer = MasterConfig->FindOrAddSettingByClass(UMoviePipelineWidgetRenderer::StaticClass()))
		{
			WidgetRenderer->bCompositeOntoFinalImage = bCompositeOntoFinalImage;
		}
	}

	if (bSettingsBurnInEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineBurnInSetting> BurnInSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineBurnInSetting::StaticClass()))
		{
			BurnInSetting->BurnInClass = BurnInClass;
			BurnInSetting->bCompositeOntoFinalImage = bBurnInCompositeOntoFinalImage;
		}
	}

	if (bSettingsCameraEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineCameraSetting> CameraSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineCameraSetting::StaticClass()))
		{
			CameraSetting->OverscanPercentage = OverscanPercentage;
			CameraSetting->ShutterTiming = ShutterTiming;
		}
	}

	if (bSettingsHighResEnabled)
	{
		if (const TSoftObjectPtr<UMoviePipelineHighResSetting> HighResSetting = MasterConfig->FindOrAddSettingByClass(UMoviePipelineHighResSetting::StaticClass()))
		{
			HighResSetting->TileCount = TileCount;
			HighResSetting->TextureSharpnessBias = TextureSharpnessBias;
			HighResSetting->OverlapRatio = OverlapRatio;
			HighResSetting->bOverrideSubSurfaceScattering = bOverrideSubSurfaceScattering;
			HighResSetting->BurleySampleCount = BurleySampleCount;
		}
	}

	return MasterConfig;
}
