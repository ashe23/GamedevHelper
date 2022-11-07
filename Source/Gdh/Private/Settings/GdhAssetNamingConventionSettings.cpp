// Copyright Ashot Barkhudaryan. All Rights Reserved.

#include "Settings/GdhAssetNamingConventionSettings.h"
#include "Gdh.h"


UGdhAssetNamingConventionSettings::UGdhAssetNamingConventionSettings()
{
}

FName UGdhAssetNamingConventionSettings::GetContainerName() const
{
	return FName{"Project"};
}

FName UGdhAssetNamingConventionSettings::GetCategoryName() const
{
	return GdhConstants::ModuleFullName;
}

FName UGdhAssetNamingConventionSettings::GetSectionName() const
{
	return FName{"Asset Naming Convention Settings"};
}

FString UGdhAssetNamingConventionSettings::GetDesc()
{
	return TEXT("GamedevHelper Asset Naming Convention Settings");
}

void UGdhAssetNamingConventionSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();

	if (OnChangeDelegate.IsBound())
	{
		OnChangeDelegate.Execute();
	}
}

FGdhAssetNamingConventionSettingsOnChangeDelegate& UGdhAssetNamingConventionSettings::OnChange()
{
	return OnChangeDelegate;
}
