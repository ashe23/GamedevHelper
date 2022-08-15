// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "ProjectSettings/GamedevHelperRenderingSettings.h"
#include "GamedevHelper.h"

UGamedevHelperRenderingSettings::UGamedevHelperRenderingSettings()
{
	OnSettingChanged().AddLambda([&](const UObject* Object, const FPropertyChangedEvent& PropertyChangedEvent)
	{
		if (!PropertyChangedEvent.MemberProperty) return;
		const FString PropertyName = PropertyChangedEvent.MemberProperty->GetName();

		if (PropertyName.Equals(TEXT("ResolutionPreset")) || PropertyName.Equals(TEXT("CustomResolution")))
		{
			switch (ResolutionPreset)
			{
				case EGamedevHelperResolutionPreset::Res360P:
					CurrentResolution = GamedevHelperConstants::Resolution360P;
				break;
				case EGamedevHelperResolutionPreset::Res480P:
					CurrentResolution = GamedevHelperConstants::Resolution480P;
				break;
				case EGamedevHelperResolutionPreset::Res720P:
					CurrentResolution = GamedevHelperConstants::Resolution720P;
				break;
				case EGamedevHelperResolutionPreset::Res1080P:
					CurrentResolution = GamedevHelperConstants::Resolution1080P;
				break;
				case EGamedevHelperResolutionPreset::Res2160P:
					CurrentResolution = GamedevHelperConstants::Resolution2160P;
				break;
				case EGamedevHelperResolutionPreset::ResCustom:
					CurrentResolution = CustomResolution;
				break;
				default:
					CurrentResolution = GamedevHelperConstants::Resolution1080P;
			}
		}
		
		UE_LOG(LogGamedevHelper, Warning, TEXT("Current Resolution: %s"), *CurrentResolution.ToString());
		UE_LOG(LogGamedevHelper, Warning, TEXT("Settings changed: %s"), *PropertyName);
	});
}

FName UGamedevHelperRenderingSettings::GetContainerName() const
{
	return FName{"Project"};
}

FName UGamedevHelperRenderingSettings::GetCategoryName() const
{
	return FName{"GamedevHelper"};
}

FName UGamedevHelperRenderingSettings::GetSectionName() const
{
	return FName{"RenderingSettings"};
}

FString UGamedevHelperRenderingSettings::GetDesc()
{
	return TEXT("GamedevHelper Rendering Settings");
}
